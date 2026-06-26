
#include "Header.h"
#include "WindowBase.h"
#include "Text.h"
#include "ButtonIcon.h"
#include "Box.h"
namespace Ling{
    WindowBase::WindowBase():compositor{ Composition::Compositor() }
    {

    }

    WindowBase::~WindowBase()
    {
    }

    void WindowBase::show()
    {
        ShowWindow(hwnd, SW_SHOW);
        //SetForegroundWindow(hwnd);
        //SetFocus(hwnd);
        //UpdateWindow(hwnd);
    }
    void WindowBase::hide()
    {
	    ShowWindow(hwnd, SW_HIDE);
    }
    void WindowBase::refresh()
    {
        InvalidateRect(hwnd, nullptr, false);
    }
    void WindowBase::close()
    {
        SetWindowLongPtr(hwnd, GWLP_USERDATA, NULL);
        DestroyWindow(hwnd);
        onDestroy();
    }
    void WindowBase::move(const int& x, const int& y)
    {
	    this->x = x;
	    this->y = y;
	    SetWindowPos(hwnd, nullptr, x, y, 0, 0, SWP_NOSIZE | SWP_NOZORDER| SWP_NOREDRAW);
    }

    void WindowBase::resize(const int& w, const int& h)
    {
	    this->w = w;
	    this->h = h;
	    SetWindowPos(hwnd, nullptr, 0, 0, w, h, SWP_NOMOVE | SWP_NOZORDER| SWP_NOREDRAW);
    }

    void WindowBase::enableShadow()
    {
        MARGINS margins = { 1,1,1,1 };
        DwmExtendFrameIntoClientArea(hwnd, &margins);
        int value = 2;
        DwmSetWindowAttribute(hwnd, DWMWA_NCRENDERING_POLICY, &value, sizeof(value));
        DwmSetWindowAttribute(hwnd, DWMWA_ALLOW_NCPAINT, &value, sizeof(value));
        DWM_WINDOW_CORNER_PREFERENCE preference = DWMWCP_DONOTROUND;
        DwmSetWindowAttribute(hwnd, DWMWA_WINDOW_CORNER_PREFERENCE, &preference, sizeof(preference));
    }
    void WindowBase::setTimer(const UINT& elapse, const UINT& id)
    {
        SetTimer(hwnd, WM_APP + id, elapse, nullptr);
    }

    void WindowBase::killTimer(const UINT& id)
    {
        KillTimer(hwnd, WM_APP + id);
    }


    void WindowBase::createNativeWindow(const DWORD& exStyle, const DWORD& style)
    {
        hwnd = CreateWindowEx(WS_EX_NOREDIRECTIONBITMAP | exStyle, getWinClsName().c_str(), NULL, style, x, y, w, h, NULL, NULL, GetModuleHandle(nullptr), NULL); //WS_POPUP
        SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(this));
	    dpi = GetDpiForWindow(hwnd) / 96.0f;
        auto interop = compositor.as<ABI::Windows::UI::Composition::Desktop::ICompositorDesktopInterop>();
        auto r = reinterpret_cast<ABI::Windows::UI::Composition::Desktop::IDesktopWindowTarget**>(winrt::put_abi(winTarget));
        interop->CreateDesktopWindowTarget(hwnd, false, r);
        body = std::make_unique<Box>(this);
        Color c(0xFFFFFFFF);
        body->visual.Brush(compositor.CreateColorBrush(c.getUIColor()));
        body->setCursor(IDC_ARROW);
        winTarget.Root(body->visual);
        onCreated();
    }

    LRESULT WindowBase::onHitTest(WPARAM wParam, LPARAM lParam)
    {
        return DefWindowProc(hwnd, WM_NCHITTEST, wParam, lParam);
    }

    BOOL WindowBase::onCursor()
    {
        if (hoverElement) {
            return hoverElement->setCursor();
        }
        return FALSE;
    }
    std::wstring& WindowBase::getWinClsName()
    {
        static std::wstring clsName = [] {
            WNDCLASSEXW wcex;
            wcex.cbSize = sizeof(WNDCLASSEX);
            wcex.style = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
            wcex.lpfnWndProc = &WindowBase::winProc;
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

    LRESULT WindowBase::winProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
    {
        auto self = reinterpret_cast<WindowBase*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
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
            if (LOWORD(lParam) == HTCLIENT) return self->onCursor();
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
            SetCapture(hwnd);
            self->isMouseDown = true;
            self->onMouseDown(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam), false);
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
            self->onTimer(wParam-WM_APP);
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

    void WindowBase::mouseMove(const int& x, const int& y)
    {
        if (!isMouseIn)
        {
            isMouseIn = true;
            TRACKMOUSEEVENT tme{ sizeof(TRACKMOUSEEVENT) };
            tme.dwFlags = TME_LEAVE;
            tme.hwndTrack = hwnd;
            TrackMouseEvent(&tme);
        }

        IElement* newHover = body->hitTest(x, y);

        if (newHover != hoverElement) {
            MouseEvent event(x, y);
            if (hoverElement) {
                // 查找老的hoverElement和新的newHover的共同祖先
                IElement* commonAncestor = hoverElement->findAncestor(newHover);
                // 从老的hoverElement向上触发mouseLeave，直到共同祖先
                IElement* current = hoverElement;
                while (current && current != commonAncestor) {
                    current->mouseLeave(event);
                    current = current->parent;
                }
            }
            if (newHover) {
                // 查找共同祖先
                IElement* commonAncestor = hoverElement ? newHover->findAncestor(hoverElement) : nullptr;
                // 从新 hover 元素向上触发 mouseEnter，直到共同祖先（不包含）
                std::vector<IElement*> pathToEnter;
                IElement* current = newHover;
                while (current && current != commonAncestor) {
                    pathToEnter.push_back(current);
                    current = current->parent;
                }
                // 从外到内触发（父元素先于子元素触发 enter）
                for (auto it = pathToEnter.rbegin(); it != pathToEnter.rend(); ++it) {
                    (*it)->mouseEnter(event);
                }
            }
            hoverElement = newHover;
        }
        if (newHover) {
            newHover->mouseMove(MouseEvent(x, y));
        }
    }

    void WindowBase::mouseLeave()
    {
	    isMouseIn = false;
        TRACKMOUSEEVENT tme{ sizeof(TRACKMOUSEEVENT) };
        tme.dwFlags = TME_CANCEL | TME_LEAVE;
        tme.hwndTrack = hwnd;
        TrackMouseEvent(&tme);
        if (hoverElement) {
            MouseEvent event(0, 0);  // 离开时坐标不重要
            IElement* current = hoverElement;
            while (current) {
                current->mouseLeave(event);
                current = current->parent;
            }
            hoverElement = nullptr;
        }
    }

    void WindowBase::sizeChange(const int& w, const int& h)
    {
        this->w = w;
        this->h = h;
        YGNodeStyleSetWidth(body->node, w);
        YGNodeStyleSetHeight(body->node, h);
        YGNodeCalculateLayout(body->node, w, h, YGDirectionLTR);
        body->layout();
    }

    void WindowBase::setTitle(const std::wstring& title)
    {
        this->title = title;
    }

    std::wstring WindowBase::getTitle()
    {
        return title;
    }
    std::tuple<int, int> WindowBase::getPosition()
    {
        return std::make_tuple(x,y);
    }

    std::tuple<float, float> WindowBase::getSize()
    {
        return std::make_tuple(w, h);
    }
    void WindowBase::setSize(const float& w, const float& h)
    {
        this->w = w;
        this->h = h;
    }

    void WindowBase::setPosition(const int& xWin, const int& yWin)
    {
        this->x = x;
        this->y = y;
    }
    HWND WindowBase::getHandle()
    {
        return hwnd;
    }
    float WindowBase::getScaleFactor()
    {
        return dpi;
    }
    void WindowBase::setPosScreenCenter()
    {
        int screenWidth = GetSystemMetrics(SM_CXSCREEN);
        int screenHeight = GetSystemMetrics(SM_CYSCREEN);
        x = (screenWidth - w) / 2 ;
        y = (screenHeight - h) / 2;
    }
    Box* WindowBase::makeBox()
    {
        auto ele = std::make_unique<Box>(this);
        auto result = ele.get();
        elements.push_back(std::move(ele));
        return result;
    }
    Text* WindowBase::makeText()
    {
        auto ele = std::make_unique<Text>(this);
        auto result = ele.get();
        elements.push_back(std::move(ele));
        return result;
    }
    ButtonIcon* WindowBase::makeButtonIcon()
    {
        auto ele = std::make_unique<ButtonIcon>(this);
        auto result = ele.get();
        elements.push_back(std::move(ele));
        return result;
    }
}