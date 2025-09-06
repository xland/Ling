#include <yoga/Yoga.h>
#include "../Include/Element.h"
#include "../Include/App.h"
#include "../Include/WindowBase.h"
#include "private/WindowBaseImpl.h"

namespace Ling {
    WindowBase::WindowBase() :winPosition(0, 0), winSize(980, 680)
    {
        //HCURSOR hArrow = LoadCursor(NULL, IDC_ARROW);
        //SetSystemCursor(hArrow, 32650);
        //SystemParametersInfo(SPI_SETCURSORS, 0, NULL, 0);
    }
    WindowBase::~WindowBase() {

    }
    void WindowBase::show() {
        ShowWindow(hwnd, SW_SHOW);     // 或者 SW_SHOWNORMAL / SW_SHOWDEFAULT
        UpdateWindow(hwnd);
        casecadeShown();
    }

    void WindowBase::resetWindowToScreenCenter()
    {
        int screenWidth = GetSystemMetrics(SM_CXSCREEN);
        int screenHeight = GetSystemMetrics(SM_CYSCREEN);
        auto size = getWindowSize();
        int x{ (screenWidth - size.w) / 2 };
        int y{ (screenHeight - size.h) / 2 };
        setWindowPosition(x, y);
        SetWindowPos(hwnd, NULL, x, y, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
    }

    void WindowBase::setWindowToScreenCenter()
    {
        int screenWidth = GetSystemMetrics(SM_CXSCREEN);
        int screenHeight = GetSystemMetrics(SM_CYSCREEN);
        auto size = getWindowSize();
        int x{ (screenWidth - size.w) / 2 };
        int y{ (screenHeight - size.h) / 2 };
        setWindowPosition(x, y);
    }

    void WindowBase::setTitle(const std::wstring& title)
    {
        this->title = title;
    }

    const std::wstring& WindowBase::getTitle()
    {
        return title;
    }

    void WindowBase::insertChild(const int& index, Element* ele)
    {
        ElementBox::insertChild(index, ele);
        casecadeSetWindow(ele);
    }

    void WindowBase::addChild(Element* ele)
    {
        ElementBox::addChild(ele);
        casecadeSetWindow(ele);
    }

    void WindowBase::casecadeSetWindow(Element* ele)
    {
        ele->setWindow(this);
        auto box = dynamic_cast<ElementBox*>(ele);
        if (box) {
            for (auto e : *(box->getChildren()))
            {
                casecadeSetWindow(e);
            }
        }
    }

    const Position& WindowBase::getWindowPosition()
    {
        return winPosition;
    }

    const Size& WindowBase::getWindowSize()
    {
        return winSize;
    }

    const Size WindowBase::getWindowClientSize()
    {
        RECT clientRect;
        GetClientRect(hwnd, &clientRect);
        Size s(clientRect.right - clientRect.left, clientRect.bottom - clientRect.top);
        return s;
    }

    void WindowBase::resetWindowSize(const int& w, const int& h)
    {
        winSize.w = w;
        winSize.h = h;
        SetWindowPos(hwnd, NULL, 0, 0, w, h, SWP_NOMOVE | SWP_NOZORDER);
    }

    void WindowBase::resetWindowPosition(const int& x, const int& y)
    {
        winPosition.x = x;
        winPosition.y = y;
        SetWindowPos(hwnd, NULL, x, y, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
    }
    void WindowBase::setWindowSize(const int& w, const int& h)
    {
        winSize.w = w;
        winSize.h = h;
    }

    void WindowBase::setWindowPosition(const int& x, const int& y)
    {
        winPosition.x = x;
        winPosition.y = y;
    }

    LRESULT CALLBACK WindowBase::routeWindowMsg(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
        auto winObj = reinterpret_cast<WindowBase*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
        if (winObj)
        {
            return winObj->windowMsgProc(hwnd, msg, wParam, lParam);
        }
        else {
            return DefWindowProc(hwnd, msg, wParam, lParam);
        }
    }

    

    void WindowBase::windowMouseMove(const int& x, const int& y)
    {
        MouseEvent e(x, y);
        auto ele = getElementByPosition(x, y);
        e.setRelativePosition(ele);
        if (hoverEle != ele) {
            if (hoverEle && !hoverEle->hittest(x, y)) {
                hoverEle->mouseLeave(e);
            }
            ele->mouseEnter(e);
            if (!hoverEle || hoverEle->getCursor() != ele->getCursor()) {
                SetCursor(LoadCursor(NULL, MAKEINTRESOURCE(ele->getCursor())));
            }
            hoverEle = ele;
        }
        ele->mouseMove(e);
    }

    void WindowBase::windowMouseDown(const int& x, const int& y, const MouseButton& mouseBtn)
    {
        MouseEvent e(x, y, mouseBtn);
        e.setRelativePosition(hoverEle);
        hoverEle->mouseDown(e);
    }

    void WindowBase::windowMouseUp(const int& x, const int& y, const MouseButton& mouseBtn)
    {
        MouseEvent e(x, y, mouseBtn);
        e.setRelativePosition(hoverEle);
        hoverEle->mouseUp(e);
    }

    void WindowBase::paintArea()
    {
        auto size = getWindowClientSize();
        auto w = size.w * scaleFactor;
        auto h = size.h * scaleFactor;
        auto pix = winImpl->getPix();
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hwnd, &ps);
        BITMAPINFO bmi{};
        bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
        bmi.bmiHeader.biWidth = w;
        bmi.bmiHeader.biHeight = -h;  // top-down
        bmi.bmiHeader.biPlanes = 1;
        bmi.bmiHeader.biBitCount = 32;
        bmi.bmiHeader.biCompression = BI_RGB;
        SetStretchBltMode(hdc, HALFTONE);
        SetBrushOrgEx(hdc, 0, 0, nullptr);
        StretchDIBits(hdc, 0, 0, size.w, size.h, 0, 0, w, h,
            pix.addr(), &bmi, DIB_RGB_COLORS, SRCCOPY);
        EndPaint(hwnd, &ps);
    }

    const std::wstring& WindowBase::getWinClsName()
    {
        static std::wstring clsName = [] {
            WNDCLASSEXW wcex;
            wcex.cbSize = sizeof(WNDCLASSEX);
            wcex.style = CS_HREDRAW | CS_VREDRAW;
            wcex.lpfnWndProc = &WindowBase::routeWindowMsg;
            wcex.cbClsExtra = 0;
            wcex.cbWndExtra = 0;
            wcex.hInstance = App::get()->hInstance;
            wcex.hIcon = LoadIcon(wcex.hInstance, (LPCTSTR)IDI_WINLOGO);
            wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
            wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
            wcex.lpszMenuName = nullptr;
            wcex.lpszClassName = L"Ling";
            wcex.hIconSm = LoadIcon(wcex.hInstance, (LPCTSTR)IDI_WINLOGO);
            RegisterClassExW(&wcex);
            return wcex.lpszClassName;
            }();
        return clsName;
    }

    void WindowBase::setScaleFactor()
    {
        UINT dpi = GetDpiForWindow(hwnd);
        scaleFactor = dpi / 96.0f;
    }

    Element* WindowBase::getElementByPosition(int x, int y)
    {
        Element* result = this;
        auto children = result->getChildren();
        while (children && children->size() > 0) {
            bool flag{ false };
            for (auto child : *children) //遍历子元素
            {
                if (child->hittest(x, y)) //命中测试
                {
                    flag = true;
                    result = child;
                    children = result->getChildren();
                    break; //结束本层级的遍历，开始遍历下一个层级，找到最底层的元素
                }
            }
            if (!flag) {
                break;//上级命中，但本级没有命中，直接退出循环。
            }
        }
        return result;
    }

    HWND WindowBase::getHandle()
    {
        return hwnd;
    }

    float WindowBase::getScaleFactor()
    {
        return scaleFactor;
    }
    void WindowBase::setFocusEle(Element* ele)
    {
        SetTimer(hwnd, FlashCaretTimer, 600, NULL); //每600毫秒触发一次
        SendMessage(hwnd, WM_TIMER, FlashCaretTimer, 0); //马上触发一次
        focusEle = ele;
    }
    size_t WindowBase::onDpiChanged(std::function<void()> callback)
    {
        dpiChangedCBId += 1;
        dpiChangedCBs.insert({ dpiChangedCBId,callback });
        return dpiChangedCBId;
    }
}
