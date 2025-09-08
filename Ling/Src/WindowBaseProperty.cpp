#include <thorvg.h>
#include "../Include/WindowBase.h"

namespace Ling {
    void WindowBase::setTitle(const std::wstring& title)
    {
        this->title = title;
    }

    const std::wstring& WindowBase::getTitle()
    {
        return title;
    }
    const Position& WindowBase::getWindowPosition()
    {
        return winPosition;
    }

    const Size& WindowBase::getWindowSize()
    {
        return winSize;
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
    HWND WindowBase::getHandle()
    {
        return hwnd;
    }
    float WindowBase::getScaleFactor()
    {
        return scaleFactor;
    }
    void WindowBase::setScaleFactor()
    {
        UINT dpi = GetDpiForWindow(hwnd);
        scaleFactor = dpi / 96.0f;
        scene->scale(scaleFactor);
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
}