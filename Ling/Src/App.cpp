#include <yoga/Yoga.h>
#include <include/core/SkFontMgr.h>
#include <include/core/SkFontStyle.h>
#include <include/ports/SkTypeface_win.h>
#include <include/core/SkFont.h>
#include <include/core/SkPaint.h>
#include <include/core/SkCanvas.h>
#include "../Include/App.h"

namespace Ling{
    std::unique_ptr<App> app;
    sk_sp<SkFontMgr> fontMgr;
    App::App(HINSTANCE hInstance):hInstance{hInstance}
    {
        auto hr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED);
        if (FAILED(hr))
        {
            MessageBox(NULL, L"Failed to initialize COM library", L"Error", MB_OK | MB_ICONERROR);
        }
    }

    App::~App() {

    }

    void App::init(HINSTANCE hInstance)
    {
        fontMgr = SkFontMgr_New_GDI();
        app = std::unique_ptr<App>(new App(hInstance));//std::make_unique<App>(hInstance);

    }

    App* App::get() {
        return app.get();
    }

    SkFontMgr* App::getFontMgr()
    {
        return fontMgr.get();
    }

    int App::exec() {
        MSG msg;
        while (GetMessage(&msg, NULL, 0, 0))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        CoUninitialize();
        return 0;
    }
}
