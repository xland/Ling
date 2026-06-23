export module Ling;

export import <string>;
export import <string_view>;
export import <ranges>;
export import <algorithm>;
export import <cwctype>;
export import <format>;
export import <map>;
export import <unordered_map>;
export import <unordered_set>;
export import <chrono>;
export import <functional>;
export import <filesystem>;
export import <fstream>;
export import <stdexcept>;

export import <math.h>;
export import <windows.h>;
//export import <windowsx.h>;
//export import <shlobj.h>;
//export import <shlwapi.h>;
//export import <wrl.h>;
//export import <wincodec.h>;
//export import <dwmapi.h>;
//export import <dispatcherqueue.h>;
//export import <winrt/Windows.UI.Composition.h>;
//export import <winrt/Windows.UI.Composition.Desktop.h>;
//export import <windows.ui.composition.interop.h>; // 如果需要互操作
//export import <unknwn.h>;
//export import <winrt/Windows.Data.Json.h>;


//using namespace Microsoft::WRL;
//using namespace winrt::Windows::Data::Json;
//using namespace winrt::Windows::UI::Composition;

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