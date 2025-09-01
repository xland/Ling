#pragma once
#include <Windows.h>
#include <stdio.h>
#include <string>
#include <stdlib.h>

#ifdef DEBUG
void InitDebuger();
#endif
std::string ConvertToUTF8(const std::wstring& wstr);
std::string ConvertToUTF8(const LPWSTR& wstr);
std::wstring ConvertToWideChar(const std::string& str);
