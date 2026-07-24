#include "pch.h"
#include "include/Ling.h"
#include "WindowWithScroller.h"

int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPTSTR lpCmdLine, _In_ int nCmdShow)
{
    Ling::init();
    WindowWithScroller win;
    win.createNativeWindow();
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    Ling::dispose();
    return 0;
}