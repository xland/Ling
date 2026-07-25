#include "pch.h"
#include "include/Ling.h"
#include "WindowWithScroller.h"
#include "WindowImage.h"
#include "WindowCanvas.h"

int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPTSTR lpCmdLine, _In_ int nCmdShow)
{
    Ling::init();
    //WindowWithScroller win;
    //WindowImage win;
    WindowCanvas win;
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    Ling::dispose();
    return 0;
}