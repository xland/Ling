#pragma once

#include <string>
#include <string_view>
#include <ranges>
#include <algorithm>
#include <cwctype>
#include <format>
#include <map>
#include <unordered_map>
#include <unordered_set>
#include <chrono>
#include <functional>
#include <filesystem>
#include <fstream>
#include <stdexcept>
#include <math.h>

#include <windows.h>
#include <windowsx.h>
#include <winuser.h>
#include <shlobj.h>
#include <shlwapi.h>
#include <wrl.h>
#include <wincodec.h>
#include <dwrite_3.h>
#include <dwmapi.h>

#include <dispatcherqueue.h>
#include <winrt/Windows.UI.Composition.h>
#include <winrt/Windows.UI.Composition.Desktop.h>
#include <winrt/Windows.Graphics.DirectX.h> // 包含像素格式枚举
#include <winrt/Windows.Graphics.DirectX.Direct3D11.h>
#include <winrt/Windows.Data.Json.h>
#include <winrt/Windows.Foundation.Collections.h>
#include <windows.ui.composition.interop.h> // 如果需要互操作

#include <unknwn.h>
using namespace Microsoft::WRL;
using namespace winrt::Windows::Data::Json;
using namespace winrt::Windows::UI;

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
