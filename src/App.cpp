#include "pch.h"
#include <Windows.h>
#include <winuser.h>
#include "../include/Util.h"
#include "../include/D2D.h"
#include "../include/App.h"
#include "../yoga/YGConfig.h"

namespace Ling {
    static std::unique_ptr<App> app;

    App::App() :dq{ winrt::Windows::System::DispatcherQueue::GetForCurrentThread() }, appID{ COMPILE_TIME_RAND_STR(6)}
    {
        SetCurrentProcessExplicitAppUserModelID(appID.data());
    }

    App::~App()
    {
        disposeTray();
    }

    App* App::get()
    {
        return app.get();
    }

    void App::quit(int code)
    {
        onBeforeQuit();
        PostQuitMessage(code);
    }

    void App::exit(int code)
    {
        ExitProcess(code);
    }

    bool App::refuseSecondInstance()
    {
        auto hwnd = FindWindow(L"STATIC", appID.data());
        if (hwnd) {
            PostMessage(hwnd, WM_APP + 1, 0, 0);
            App::exit(0);
            return true;
        }
        initMsgWin();
        return false;
    }

    void App::regHotKey(const std::wstring& keyStr, const UINT msgId)
    {
        std::wstring lowerName = keyStr;
        std::transform(lowerName.begin(), lowerName.end(), lowerName.begin(), ::towlower);
        auto arr = Util::splitStr(lowerName, L'+');
        UINT modifiers = 0;
        UINT keyCode{ 0 };
        for (auto& key : arr)
        {
            if (key == L"ctrl") {
                modifiers |= MOD_CONTROL;
            }
            else if (key == L"alt") {
                modifiers |= MOD_ALT;
            }
            else if (key == L"shift") {
                modifiers |= MOD_SHIFT;
            }
            else if (key == L"win" || key == L"lwin" || key == L"rwin") {
                modifiers |= MOD_WIN;
            }
            else {
                keyCode = Util::strToKey(key);
            }
        }
        if (keyCode == 0) {
            _ASSERT_EXPR(FALSE, L"can not recognize key code，error");
            return;
        }
        initMsgWin();
        BOOL result = RegisterHotKey(msgHwnd, WM_APP + msgId, modifiers, keyCode);
        //if (!result) {
        //	DWORD error = GetLastError();
        //	if (error == ERROR_HOTKEY_ALREADY_REGISTERED) {
        //        _ASSERT_EXPR(FALSE, L"hot key confilict，error");
        //	}
        //}
    }

    void App::unRegHotKey(const UINT msgId)
    {
        UnregisterHotKey(msgHwnd, WM_APP + msgId);
    }

    void App::initTray(const UINT msgId, const std::wstring& tip)
    {
        if (tray.get()) return;
        initMsgWin();
        tray = std::make_unique<NOTIFYICONDATA>();
        ZeroMemory(tray.get(), sizeof(NOTIFYICONDATA));
        tray->cbSize = sizeof(NOTIFYICONDATA);
        tray->hWnd = msgHwnd; 
        tray->uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
        trayMsgId = WM_APP + msgId;
        tray->uCallbackMessage = trayMsgId;
        tray->hIcon = LoadIcon(GetModuleHandle(nullptr), MAKEINTRESOURCE(1));
        if (!tip.empty()) {
            wcscpy_s(tray->szTip, tip.data());
        }        
        Shell_NotifyIcon(NIM_ADD, tray.get());
    }

    void App::disposeTray()
    {
        if (tray.get()) {
            Shell_NotifyIcon(NIM_DELETE, tray.get());
            tray.reset();
        }
    }

    void App::initArgs()
    {
        LPWSTR* argv;
        int argc;
        LPWSTR cmdLine = GetCommandLine();
        argv = CommandLineToArgvW(cmdLine, &argc);
        for (int i = 1; i < argc; ++i) {
            std::wstring arg{ argv[i] };
            auto index = arg.find(L"=");
            if (index != std::wstring::npos) {
                args[arg.substr(0, index)] = arg.substr(index + 1);
            }
            else {
                args.insert({ arg,L"true" });
            }
        }
        LocalFree(argv);
    }

    UINT App::popupMenu(HMENU menu)
    {
        POINT pt;
        GetCursorPos(&pt);
        // 参考 MS KB 135788：托盘弹菜单前必须把 owner 拉到前台, 菜单才能收到"外部点击"
        // 和"失焦"事件从而自动关闭；否则会一直挂着。菜单结束后再补一个 WM_NULL, 兜住
        // 部分 shell 版本的兼容问题。
        SetForegroundWindow(msgHwnd);
        UINT selectedCmd = TrackPopupMenuEx(menu,TPM_RIGHTBUTTON | TPM_NONOTIFY | TPM_RETURNCMD,pt.x,pt.y,msgHwnd,nullptr);
        PostMessage(msgHwnd, WM_NULL, 0, 0);
        DestroyMenu(menu);
        return selectedCmd;
    }

    void App::init()
    {
        App::initDispatcherQueueCtrl();
        D2D::init();
        auto ptr = new App();
        app.reset(ptr);
    }

    void App::initMsgWin()
    {
        if (msgHwnd) return;
        msgHwnd = CreateWindow(L"STATIC", appID.data(), 0, 0, 0, 0, 0, HWND_MESSAGE, NULL, GetInstanceModule(NULL), NULL);
        SetWindowLongPtr(msgHwnd, GWLP_WNDPROC, (LONG_PTR)App::winProc);
        SetWindowLongPtr(msgHwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(this));
    }

    LRESULT App::winProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
    {
        auto self = reinterpret_cast<App*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
        if (!self) {
            return DefWindowProc(hwnd, msg, wParam, lParam);
        }
        else if (msg == WM_APP + 1)
        {
            self->onSecondInstance();
            return 0;
        }
        else if (msg == WM_HOTKEY) {
            self->onHotKey((UINT)wParam - WM_APP);
            return 0;
        }
        else if (msg == self->trayMsgId)
        {
            if (lParam == WM_LBUTTONDOWN) {
                self->onTrayMouseEvent(true,false);
            }
            else if (lParam == WM_RBUTTONDOWN)
            {
                self->onTrayMouseEvent(true, true);
            }
            else if (lParam == WM_LBUTTONUP)
            {
                self->onTrayMouseEvent(false, false);
            }
            else if (lParam == WM_RBUTTONUP)
            {
                self->onTrayMouseEvent(false, true);
            }
        }
        return DefWindowProc(hwnd, msg, wParam, lParam);
    }

    void App::initDispatcherQueueCtrl()
    {
        DispatcherQueueOptions options{ sizeof(DispatcherQueueOptions), DQTYPE_THREAD_CURRENT, DQTAT_COM_NONE };
        static winrt::Windows::System::DispatcherQueueController controller{ nullptr };
        auto hr = CreateDispatcherQueueController(options, reinterpret_cast<ABI::Windows::System::IDispatcherQueueController**>(winrt::put_abi(controller)));
        if (FAILED(hr))
        {
            MessageBox(NULL, L"无法创建DispatcherQueueController", L"系统提示", MB_OK);
            ExitProcess(-1);
        }
    }
}
