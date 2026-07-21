#include "pch.h"
#include "../include/App.h"

namespace Ling {
    static std::unique_ptr<App> app;

    App::App() :dq{ winrt::Windows::System::DispatcherQueue::GetForCurrentThread() }
    {
    }

    App::~App()
    {
    }

    App* App::get()
    {
        return app.get();
    }

    void App::init()
    {
        App::initDispatcherQueueCtrl();
        auto ptr = new App();
        app.reset(ptr);
    }

    void App::initDispatcherQueueCtrl()
    {
        DispatcherQueueOptions options{ sizeof(DispatcherQueueOptions), DQTYPE_THREAD_CURRENT, DQTAT_COM_NONE };
        static winrt::Windows::System::DispatcherQueueController controller{ nullptr };
        auto hr = CreateDispatcherQueueController(options, reinterpret_cast<ABI::Windows::System::IDispatcherQueueController**>(winrt::put_abi(controller)));
        if (FAILED(hr))
        {
            MessageBox(NULL, L"无法创建DispatcherQueueController", L"系统提示", MB_OK);
            ExitProcess(-1);
        }
    }
}
