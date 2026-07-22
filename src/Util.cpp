#include "pch.h"
#include "../include/Util.h"

namespace Ling {
    bool Util::isWin11()
    {
        RTL_OSVERSIONINFOW osInfo = { 0 };
        osInfo.dwOSVersionInfoSize = sizeof(osInfo);
        HMODULE hNtDll = GetModuleHandle(L"ntdll.dll");
        if (!hNtDll) return false;
        typedef LONG(WINAPI* RtlGetVersionPtr)(PRTL_OSVERSIONINFOW);
        RtlGetVersionPtr RtlGetVersion = (RtlGetVersionPtr)GetProcAddress(hNtDll, "RtlGetVersion");
        if (RtlGetVersion == NULL) return false;
        if (RtlGetVersion(&osInfo) != 0) return false;
        if (osInfo.dwMajorVersion == 10 && osInfo.dwMinorVersion == 0) {
            if (osInfo.dwBuildNumber >= 22000) {
                return true;
            }
            else
            {
                return false;
            }
        }
        return false;
    }

    std::wstring Util::convertToWStr(const char* str)
    {
        if (!str) return std::wstring();
        int count = MultiByteToWideChar(CP_UTF8, 0, str, -1, 0, 0);
        if (count == 0) return std::wstring();
        std::vector<wchar_t> buffer(count);
        MultiByteToWideChar(CP_UTF8, 0, str, -1, buffer.data(), count);
        return std::wstring(buffer.data(), buffer.size() - 1);
    }

    void Util::setTextToClipboard(const std::wstring& text)
    {
        if (!OpenClipboard(NULL)) return;
        EmptyClipboard();
        size_t length = (text.size() + 1) * sizeof(wchar_t);
        HGLOBAL hGlobal = GlobalAlloc(GMEM_MOVEABLE, length);
        if (hGlobal == NULL) {
            CloseClipboard();
            return;
        }
        auto pGlobal = (wchar_t*)GlobalLock(hGlobal);
        if (pGlobal == NULL) {
            CloseClipboard();
            return;
        }
        memcpy(pGlobal, text.data(), length);
        GlobalUnlock(hGlobal);
        SetClipboardData(CF_UNICODETEXT, hGlobal);
        CloseClipboard();
    }

    std::tuple<void*, DWORD> Util::getRes(const std::wstring& name)
    {
        HRSRC hRes = FindResource(NULL, name.data(), RT_RCDATA);
        if (!hRes) {
            return std::make_tuple(nullptr, 0);
        }
        HGLOBAL hData = LoadResource(NULL, hRes);
        if (!hData) {
            return std::make_tuple(nullptr, 0);
        }
        void* pData = LockResource(hData);
        DWORD size = SizeofResource(NULL, hRes);
        return std::make_tuple(pData, size);
    }

}

