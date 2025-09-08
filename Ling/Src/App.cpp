#include <yoga/Yoga.h>
#include <thorvg.h>
#include <thread>
#include "../Include/App.h"

namespace Ling{
    std::unique_ptr<App> app;
    App::App(HINSTANCE hInstance):hInstance{hInstance}
    {
        tvg::Initializer::init(std::thread::hardware_concurrency());
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
        app = std::unique_ptr<App>(new App(hInstance));//std::make_unique<App>(hInstance);

    }

    App* App::get() {
        return app.get();
    }

    int App::exec() {
        MSG msg;
        while (GetMessage(&msg, NULL, 0, 0))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        CoUninitialize();
        tvg::Initializer::term();
        return 0;
    }
    void App::loadFont(const std::string& fontName)
    {
        if (fontName == "Arial") {
            auto r = tvg::Text::load("C:\\Windows\\Fonts\\Arial.ttf");
            auto a = r;
        }
        else if (fontName == "Microsoft YaHei") {
            auto r = tvg::Text::load("C:\\Windows\\Fonts\\msyh.ttc");
            auto a = r;
        }
        else if (fontName == "Segoe UI") {
            tvg::Text::load("C:\\Windows\\Fonts\\segoeui.ttf");
        }
        else if (fontName == "Microsoft JhengHei") {
			tvg::Text::load("C:\\Windows\\Fonts\\msjh.ttc"); // 微软正黑体 繁体
        }
        else if (fontName == "SimSun") {
            tvg::Text::load("C:\\Windows\\Fonts\\simsun.ttc"); // 宋体
        }
        else if (fontName == "SimHei") {
            auto r = tvg::Text::load("C:\\Windows\\Fonts\\SimHei.ttf"); // 黑体
            auto a = r;
        }
        else if (fontName == "SimKai") {
            tvg::Text::load("C:\\Windows\\Fonts\\simkai.ttf"); // 楷体
        }
        else if (fontName == "SimFang") {
            tvg::Text::load("C:\\Windows\\Fonts\\simfang.ttf"); //仿宋
        }
        else if (fontName == "Segoe UI Emoji") {
            tvg::Text::load("C:\\Windows\\Fonts\\seguiemj.ttf"); //表情
        }
    }
}
