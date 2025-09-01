#include "../Include/App.h"

namespace Ling {
    std::unique_ptr<App> app;
    App::App(HINSTANCE hInstance) :hInstance{ hInstance }
    {
        auto hr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED);
        if (FAILED(hr))
        {
            MessageBox(NULL, L"Failed to initialize COM library", L"Error", MB_OK | MB_ICONERROR);
        }
    }
    void App::init(HINSTANCE hInstance)
    {
        app = std::unique_ptr<App>(new App(hInstance));
    }
    App* App::get()
    {
        return app.get();
    }
    int App::exec()
    {
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

