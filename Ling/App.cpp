#include "pch.h"
#include "App.h"

namespace Ling {
    Ling::App::App()
    {
    }

    void Ling::App::init()
    {
        winrt::init_apartment();
        //必须执行以下操作，才能使用ui元素
        DispatcherQueueOptions options{ sizeof(DispatcherQueueOptions), DQTYPE_THREAD_CURRENT, DQTAT_COM_NONE };
        static winrt::Windows::System::DispatcherQueueController controller{ nullptr };
        auto hr = CreateDispatcherQueueController(options, reinterpret_cast<ABI::Windows::System::IDispatcherQueueController**>(winrt::put_abi(controller)));
        if (FAILED(hr))
        {
            MessageBox(NULL, L"无法创建DispatcherQueueController", L"系统提示", MB_OK);
            ExitProcess(-1);
        }
    }

    Ling::App::~App()
    {
    }
}

