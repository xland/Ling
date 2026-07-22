#pragma once
#include <string>
#include <Windows.h>

namespace Ling {
	class Util
	{
	public:
		static bool isWin11();
		static std::wstring convertToWStr(const char* str);
		static void setTextToClipboard(const std::wstring& text);
		static std::tuple<void*, DWORD> getRes(const std::wstring& name);
	};
}