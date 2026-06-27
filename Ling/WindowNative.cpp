#include "WindowNative.h"
#include "WindowBase.h"
#include "Box.h"

namespace Ling {
    WindowNative::WindowNative():WindowElementManager()
    {

    }

    WindowNative::~WindowNative()
    {

    }

    void WindowNative::show()
    {
        ShowWindow(hwnd, SW_SHOW);
        //SetForegroundWindow(hwnd);
        //SetFocus(hwnd);
        //UpdateWindow(hwnd);
    }
    void WindowNative::hide()
    {
        ShowWindow(hwnd, SW_HIDE);
    }
    void WindowNative::refresh()
    {
        InvalidateRect(hwnd, nullptr, false);
    }
    void WindowNative::close()
    {
        SetWindowLongPtr(hwnd, GWLP_USERDATA, NULL);
        DestroyWindow(hwnd);
        onDestroy();
    }
    void WindowNative::move(const int& x, const int& y)
    {
        this->x = x;
        this->y = y;
        SetWindowPos(hwnd, nullptr, x, y, 0, 0, SWP_NOSIZE | SWP_NOZORDER | SWP_NOREDRAW);
    }

    void WindowNative::resize(const int& w, const int& h)
    {
        this->w = w;
        this->h = h;
        SetWindowPos(hwnd, nullptr, 0, 0, w, h, SWP_NOMOVE | SWP_NOZORDER | SWP_NOREDRAW);
    }

    void WindowNative::enableShadow()
    {
        MARGINS margins = { 1,1,1,1 };
        DwmExtendFrameIntoClientArea(hwnd, &margins);
        int value = 2;
        DwmSetWindowAttribute(hwnd, DWMWA_NCRENDERING_POLICY, &value, sizeof(value));
        DwmSetWindowAttribute(hwnd, DWMWA_ALLOW_NCPAINT, &value, sizeof(value));
        DWM_WINDOW_CORNER_PREFERENCE preference = DWMWCP_DONOTROUND;
        DwmSetWindowAttribute(hwnd, DWMWA_WINDOW_CORNER_PREFERENCE, &preference, sizeof(preference));
    }
    void WindowNative::setTimer(const UINT& elapse, const UINT& id)
    {
        SetTimer(hwnd, WM_APP + id, elapse, nullptr);
    }

    void WindowNative::killTimer(const UINT& id)
    {
        KillTimer(hwnd, WM_APP + id);
    }


    void WindowNative::setTitle(const std::wstring& title)
    {
        this->title = title;
    }

    std::wstring WindowNative::getTitle()
    {
        return title;
    }
    std::tuple<int, int> WindowNative::getPosition()
    {
        return std::make_tuple(x, y);
    }

    std::tuple<float, float> WindowNative::getSize()
    {
        return std::make_tuple(w, h);
    }
    void WindowNative::setSize(const float& w, const float& h)
    {
        this->w = w;
        this->h = h;
    }

    void WindowNative::setPosition(const int& xWin, const int& yWin)
    {
        this->x = x;
        this->y = y;
    }
    HWND WindowNative::getHandle()
    {
        return hwnd;
    }
    float WindowNative::getScaleFactor()
    {
        return dpi;
    }
    void WindowNative::setPosScreenCenter()
    {
        int screenWidth = GetSystemMetrics(SM_CXSCREEN);
        int screenHeight = GetSystemMetrics(SM_CYSCREEN);
        x = (screenWidth - w) / 2;
        y = (screenHeight - h) / 2;
    }

    void WindowNative::createNativeWindow(const DWORD& exStyle, const DWORD& style)
    {
        hwnd = CreateWindowEx(WS_EX_NOREDIRECTIONBITMAP | exStyle, getWinClsName().c_str(), NULL, style, x, y, w, h, NULL, NULL, GetModuleHandle(nullptr), NULL); //WS_POPUP
        SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(this));
        dpi = GetDpiForWindow(hwnd) / 96.0f;
        auto interop = compositor.as<ABI::Windows::UI::Composition::Desktop::ICompositorDesktopInterop>();
        auto r = reinterpret_cast<ABI::Windows::UI::Composition::Desktop::IDesktopWindowTarget**>(winrt::put_abi(winTarget));
        interop->CreateDesktopWindowTarget(hwnd, false, r);
        initBody();
        winTarget.Root(body->visual);
        onCreated();
    }

    LRESULT WindowNative::onHitTest(WPARAM wParam, LPARAM lParam)
    {
        return DefWindowProc(hwnd, WM_NCHITTEST, wParam, lParam);
    }

    std::wstring& WindowNative::getWinClsName()
    {
        static std::wstring clsName = [] {
            WNDCLASSEXW wcex;
            wcex.cbSize = sizeof(WNDCLASSEX);
            wcex.style = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
            wcex.lpfnWndProc = &WindowNative::winProc;
            wcex.cbClsExtra = 0;
            wcex.cbWndExtra = 0;
            wcex.hInstance = GetModuleHandle(nullptr);
            wcex.hIcon = LoadIcon(wcex.hInstance, (LPCTSTR)IDI_WINLOGO);
            wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
            wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
            wcex.lpszMenuName = nullptr;
            wcex.lpszClassName = L"Ling";
            wcex.hIconSm = LoadIcon(wcex.hInstance, (LPCTSTR)IDI_WINLOGO);
            auto r = RegisterClassEx(&wcex);
            if (r == 0) {
                log(L"err:: reg window class error");
            }
            return wcex.lpszClassName;
            }();
        return clsName;
    }

    LRESULT WindowNative::winProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
    {
        auto self = reinterpret_cast<WindowNative*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
        if (!self) {
            return DefWindowProc(hwnd, msg, wParam, lParam);
        }
        else if (msg == WM_MOUSEACTIVATE) {
            return MA_NOACTIVATE;
        }
        else if (msg == WM_NCHITTEST)
        {
            return self->onHitTest(wParam, lParam);
        }
        else if (msg == WM_ERASEBKGND)
        {
            return 1;
        }
        else if (msg == WM_SHOWWINDOW) {
            if (wParam) {
                self->onShown();
            }
            else {
                self->onHidden();
            }
        }
        else if (msg == WM_SETCURSOR) {
            if (LOWORD(lParam) == HTCLIENT) return self->setCursor();
        }
        else if (msg == WM_RBUTTONDOWN)
        {
            self->onMouseDown(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam), true);
        }
        else if (msg == WM_RBUTTONDBLCLK)
        {
            self->onMouseDoubleClick(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam), true);
            return 0;
        }
        else if (msg == WM_LBUTTONDBLCLK)
        {
            self->onMouseDoubleClick(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam), false);
            return 0;
        }
        else if (msg == WM_LBUTTONDOWN) {
            self->mouseDown(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam), false);
        }
        else if (msg == WM_LBUTTONUP) {
            ReleaseCapture();
            self->isMouseDown = false;
            self->onMouseUp(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
        }
        else if (msg == WM_MOUSEMOVE) {
            if (self->isMouseDown) {
                self->onMouseDrag(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam), wParam);
            }
            else {
                self->mouseMove(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
            }
        }
        else if (msg == WM_MOUSELEAVE) {
            self->mouseLeave();
            return 0;
        }
        else if (msg == WM_MOUSEWHEEL) {
            POINT pt{ (short)GET_X_LPARAM(lParam), (short)GET_Y_LPARAM(lParam) };
            ScreenToClient(hwnd, &pt);
            self->onMouseWheel(pt.x, pt.y, (short)HIWORD(wParam));
            return 0;
        }
        else if (msg == WM_KEYDOWN) {
            self->onKeyDown(wParam);
        }
        else if (msg == WM_KEYUP) {
            self->onKeyUp();
        }
        else if (msg == WM_CHAR) {
            self->onChar(wParam);
        }
        else if (msg == WM_IME_STARTCOMPOSITION) {
            self->onIme();
        }
        else if (msg == WM_TIMER) {
            self->onTimer(wParam - WM_APP);
        }
        else if (msg == WM_KILLFOCUS) {
            self->onBlur();
        }
        else if (msg == WM_DPICHANGED) {
            self->onDpiChanged();
            return 0;
        }
        else if (msg == WM_SIZE) {
            self->sizeChange(LOWORD(lParam), HIWORD(lParam));
            return 0;
        }
        return DefWindowProc(hwnd, msg, wParam, lParam);
    }

    void WindowNative::mouseMove(const int& x, const int& y)
    {
        if (!isMouseIn)
        {
            isMouseIn = true;
            TRACKMOUSEEVENT tme{ sizeof(TRACKMOUSEEVENT) };
            tme.dwFlags = TME_LEAVE;
            tme.hwndTrack = hwnd;
            TrackMouseEvent(&tme);
        }
        mouseEnterElement(x,y);
    }

    void WindowNative::mouseLeave()
    {
        isMouseIn = false;
        TRACKMOUSEEVENT tme{ sizeof(TRACKMOUSEEVENT) };
        tme.dwFlags = TME_CANCEL | TME_LEAVE;
        tme.hwndTrack = hwnd;
        TrackMouseEvent(&tme);
        mouseLeaveElement();
    }

    void WindowNative::mouseDown(const int& x, const int& y, bool isRight)
    {
        SetCapture(hwnd);
        isMouseDown = true;
        onMouseDown(x, y, false);
        hoverElement->mouseDown(MouseEvent(x, y, MouseButton::Left));
    }

    void WindowNative::sizeChange(const int& w, const int& h)
    {
        this->w = w;
        this->h = h;
        layout((float)w,(float)h);
    }
}