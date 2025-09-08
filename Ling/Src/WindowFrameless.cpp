#include <dwmapi.h>
#include "../Include/App.h"
#include "../Include/WindowFrameless.h"

namespace Ling {
    WindowFrameless::WindowFrameless()
    {
    }

    WindowFrameless::~WindowFrameless()
    {
    }


    void WindowFrameless::createNativeWindow()
    {
        auto pos = getWindowPosition();
        auto size = getWindowSize();
        hwnd = CreateWindowEx(WS_EX_APPWINDOW | WS_EX_WINDOWEDGE, getWinClsName().data(), title.data(), WS_POPUP,
            pos.x, pos.y, size.w, size.h, nullptr, nullptr, App::get()->hInstance, nullptr);
        SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(this));
        MARGINS margins = { 0, 0, 0, 1 };
        DwmExtendFrameIntoClientArea(hwnd, &margins);
        int value = 2;
        DwmSetWindowAttribute(hwnd, DWMWA_NCRENDERING_POLICY, &value, sizeof(value));
        DwmSetWindowAttribute(hwnd, DWMWA_ALLOW_NCPAINT, &value, sizeof(value));
        setScaleFactor();
        resetCanvas();
    }

    LRESULT WindowFrameless::customMsgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
    {
        switch (msg)
        {
            case WM_NCHITTEST:
            {
                int x = GET_X_LPARAM(lParam);
                int y = GET_Y_LPARAM(lParam);
                return this->hitTest(x, y);
                break;
            }
        }
        return WindowBase::customMsgProc(hwnd, msg, wParam, lParam);
    }

    LRESULT WindowFrameless::hitTest(int x, int y) {
        if (!hwnd) return HTNOWHERE;
        RECT winRect;
        GetWindowRect(hwnd, &winRect);
        if (x > winRect.left && y > winRect.top && x < winRect.right && y < winRect.bottom) {
            if (resizable) {
                int borderWidth = 5;
                if (x < winRect.left + borderWidth && y < winRect.top + borderWidth) return HTTOPLEFT;
                else if (x < winRect.left + borderWidth && y > winRect.bottom - borderWidth) return HTBOTTOMLEFT;
                else if (x > winRect.right - borderWidth && y > winRect.bottom - borderWidth) return HTBOTTOMRIGHT;
                else if (x > winRect.right - borderWidth && y < winRect.top + borderWidth) return HTTOPRIGHT;
                else if (x < winRect.left + borderWidth) return HTLEFT;
                else if (x > winRect.right - borderWidth) return HTRIGHT;
                else if (y < winRect.top + borderWidth) return HTTOP;
                else if (y > winRect.bottom - borderWidth) return HTBOTTOM;
            }
            auto ele = getElementByPosition(x - winRect.left, y - winRect.top);
            if (ele->getCaptionFlag()) {
                return HTCAPTION;
            }
            return HTCLIENT;
        }
        else
        {
            return HTNOWHERE;
        }
    }
}
