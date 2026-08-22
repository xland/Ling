#pragma once
#include <string>
#include <Windows.h>
#include <fstream>
#include <vector>

namespace Ling {
#define COMPILE_TIME_RAND_STR(LEN) \
    []<size_t... I>(std::index_sequence<I...>) -> std::wstring { \
        constexpr std::wstring_view chars = L"0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz"; \
        /* 将 I 作为显式参数 idx 传入，而不是在内层 lambda 中直接捕获 I */ \
        constexpr auto gen = [](size_t i, size_t idx) { \
            uint64_t s = static_cast<uint64_t>(__LINE__) * 31 + __COUNTER__ * 17 + __TIME__[i % 8] * (i + 1); \
            s = s * 6364136223846793005ULL + 1442695040888963407ULL; \
            return chars[(s >> (idx * 3)) % chars.size()]; \
        }; \
        /* 在展开 I 时，将当前的 I 作为实参传给 gen */ \
        return L"Ling_" + std::wstring{gen(I, I)...}; \
    }(std::make_index_sequence<LEN>{})


	class Util
	{
	public:
		static bool isWin11();
		static std::wstring convertToWStr(const char* str);
        static std::string convertToStr(const std::wstring& wstr);
		static void setTextToClipboard(const std::wstring& text);
		static std::wstring getTextFromClipboard();
		static std::tuple<void*, DWORD> getRes(const std::wstring& name);
		static std::vector<std::wstring> splitStr(const std::wstring& str, wchar_t delimiter);
		static UINT strToKey(const std::wstring& vkCode);
        static std::wstring getSysLang();
        static std::wstring readFile(const std::wstring& path);
        static void saveFile(const std::wstring& path, const std::wstring& content);
        static std::array<int, 3> getVerNum(const std::wstring& exePath = L"");
        static std::wstring readTextFromBytes(const void* data, size_t size);
        static std::wstring readFileText(const std::filesystem::path& path);

        template <std::ranges::input_range Range, typename T>
        static int getIndex(const Range& range, const T& target) {
            auto it = std::ranges::find(range, target);
            if (it != std::ranges::end(range)) {
                return std::ranges::distance(range.begin(), it);
            }
            return -1;
        }
	};
}