#include "pch.h"
#include <Windows.h>
#include "../include/D2D.h"
#include "../include/App.h"
#include "../yoga/YGConfig.h"

namespace Ling {
    static std::unique_ptr<App> app;

    App::App() :dq{ winrt::Windows::System::DispatcherQueue::GetForCurrentThread() }
    {
    }

    App::~App()
    {
    }

    App* App::get()
    {
        return app.get();
    }

    void App::quit(int code)
    {
        PostQuitMessage(code);
    }

    void App::exit(int code)
    {
        ExitProcess(code);
    }

    void App::refuseSecondInstance(const std::wstring& checkId)
    {
        _ASSERT_EXPR(msgHwnd == nullptr, L"too late to refuse second instance");
        auto msgWndName = std::format(L"Ling_{}", checkId);
        auto hwnd = FindWindow(L"STATIC", msgWndName.data());
        if (hwnd) {
            PostMessage(hwnd, WM_APP + 660, 0, 0);
            App::exit(0);
            return;
        }
        initMsgWin(msgWndName);
    }

    void App::init()
    {
        App::initDispatcherQueueCtrl();
        D2D::init();
        auto ptr = new App();
        app.reset(ptr);
    }

    void App::initMsgWin(const std::wstring& msgWndName)
    {
        if (msgHwnd) return;
        msgHwnd = CreateWindow(L"STATIC", msgWndName.data(), 0, 0, 0, 0, 0, HWND_MESSAGE, NULL, GetInstanceModule(NULL), NULL);
        SetWindowLongPtr(msgHwnd, GWLP_WNDPROC, (LONG_PTR)App::winProc);
        SetWindowLongPtr(msgHwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(this));
    }

    LRESULT App::winProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
    {
        auto self = reinterpret_cast<App*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
        if (!self) {
            return DefWindowProc(hwnd, msg, wParam, lParam);
        }
        else if (msg == WM_APP + 660)
        {
            self->onSecondInstance();
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
