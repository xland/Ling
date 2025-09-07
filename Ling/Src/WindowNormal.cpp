#include <dwmapi.h>

#include "../Include/App.h"
#include "../Include/WindowNormal.h"
#include "../Include/Util.h"
#include "private\WindowBaseImpl.h"


namespace Ling {
    void WindowNormal::createNativeWindow()
    {
        auto pos = getWindowPosition();
        auto size = getWindowSize();
        hwnd = CreateWindowEx(WS_EX_APPWINDOW, getWinClsName().data(), title.data(), WS_OVERLAPPEDWINDOW,
            pos.x, pos.y, size.w, size.h, nullptr, nullptr, App::get()->hInstance, nullptr);
        SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(this));
        onWindowCreated();
    }
}
