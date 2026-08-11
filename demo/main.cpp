#include "pch.h"
#include "include/Ling.h"
#include "WindowScroller.h"
#include "WindowImage.h"
#include "WindowCanvas.h"
#include "WindowBorderRadius.h"
#include "WindowSlider.h"
#include "WindowTextBox.h"

int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPTSTR lpCmdLine, _In_ int nCmdShow)
{
    Ling::init();
    Ling::App::get()->refuseSecondInstance();
    Ling::D2D::get()->addFonts({ L"icon.ttf" });

    //WindowScroller win;
    //WindowImage win;
    //WindowCanvas win;
    //WindowBorderRadius win;
    //WindowSlider win;
    WindowTextBox win;
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    Ling::dispose();
    return 0;
}