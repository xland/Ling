#include "App.h"
#include "WindowNormal.h"
#include "Util.h"
#include "dwmapi.h"
#include "private\WindowBaseImpl.h"



void WindowNormal::createNativeWindow()
{
    auto pos = getWindowPosition();
    auto size = getWindowSize();
    hwnd = CreateWindowEx(WS_EX_APPWINDOW, getWinClsName().data(), title.data(), WS_OVERLAPPEDWINDOW,
        pos.x, pos.y, size.w, size.h, nullptr, nullptr, App::get()->hInstance, nullptr);
    SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(this));
    setScaleFactor();
    size = getWindowClientSize();
    setSize(size.w, size.h);
    winImpl = std::make_unique<WindowBaseImpl>(this);
}

