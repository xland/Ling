#pragma once

#include <windows.h>
#include <windowsx.h>
#include <shellscalingapi.h>
#include <d2d1_1.h>
#include <dwrite_3.h>
#include <d3d11.h>
#include <dwmapi.h>
#include <shlobj.h>
#include <shlwapi.h>
#include <wrl.h>
#include <filesystem>
#include <string>
#include <format>
#include <unordered_map>
#include <unordered_set>
#include <functional>
#include <iostream>
#include <sstream>
#include <algorithm>

#include <dispatcherqueue.h>
#include <winrt/Windows.System.h>
#include <winrt/Windows.Networking.Sockets.h>
#include <winrt/Windows.Foundation.h>
#include <winrt/Windows.Data.Json.h>
#include <winrt/Windows.Foundation.Collections.h>
#include <winrt/Windows.Storage.h>
#include <winrt/Windows.Storage.Pickers.h>
#include <winrt/Windows.Storage.Streams.h>
#include <winrt/Windows.Storage.AccessCache.h>
#include <winrt/Windows.UI.Composition.h>
#include <winrt/Windows.UI.Composition.Desktop.h>
#include <winrt/Windows.Graphics.DirectX.h> // 包含像素格式枚举
#include <winrt/Windows.Graphics.DirectX.Direct3D11.h>
#include <Windows.UI.Composition.Interop.h> // 如果需要互操作

using namespace winrt::Windows::Data::Json;
using namespace winrt::Windows::Storage;
using namespace winrt::Windows::Networking::Sockets;
using namespace winrt::Windows::UI;
using namespace Microsoft::WRL;

template<typename... Args>
void log(std::wstring_view fmt, Args&&... args)
{
    auto now = std::chrono::system_clock::now();
    auto time = std::chrono::system_clock::to_time_t(now);
    std::tm tm{};
    localtime_s(&tm, &time);
    std::wstringstream ss;
    ss << std::put_time(&tm, L"%Y-%m-%d %H:%M:%S");
    auto timeStr = ss.str();
    auto msg = std::vformat(fmt, std::make_wformat_args(args...));
    std::wstring final = L"[" + timeStr + L"] " + msg + L"\n";
    OutputDebugString(final.c_str());
}
