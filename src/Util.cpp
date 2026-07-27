#include "pch.h"
#include <cwctype>
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
    std::vector<std::wstring> Util::splitStr(const std::wstring& str, wchar_t delimiter) {
        std::vector<std::wstring> result;
        std::wistringstream wiss(str);
        std::wstring token;
        while (std::getline(wiss, token, delimiter)) {
            result.push_back(token);
        }
        return result;
    }
    UINT Util::strToKey(const std::wstring& vkCode)
    {
        if (vkCode.empty()) return 0;
        // 1. 处理单字符 (A-Z, 0-9)
        if (vkCode.length() == 1) {
            wchar_t c = vkCode[0];
            if (c >= 'A' && c <= 'Z') return static_cast<UINT>(c);
            if (c >= 'a' && c <= 'z') return static_cast<UINT>(std::towupper(c));
            if (c >= '0' && c <= '9') return static_cast<UINT>(c);
        }
        // 2. 处理小键盘数字 "Num0" 等
        if (vkCode.rfind(L"num", 0) == 0) {
            std::wstring numPart = vkCode.substr(vkCode.find_first_not_of(L" \t", 3));
            if (!numPart.empty() && numPart.length() == 1) {
                wchar_t c = numPart[0];
                if (c >= '0' && c <= '9') {
                    return VK_NUMPAD0 + (c - '0');
                }
            }
        }
        // 3. 功能键 (F1 - F12)
        if (vkCode == L"f1") return VK_F1;
        if (vkCode == L"f2") return VK_F2;
        if (vkCode == L"f3") return VK_F3;
        if (vkCode == L"f4") return VK_F4;
        if (vkCode == L"f5") return VK_F5;
        if (vkCode == L"f6") return VK_F6;
        if (vkCode == L"f7") return VK_F7;
        if (vkCode == L"f8") return VK_F8;
        if (vkCode == L"f9") return VK_F9;
        if (vkCode == L"f10") return VK_F10;
        if (vkCode == L"f11") return VK_F11;
        if (vkCode == L"f12") return VK_F12;
        // 4. 方向键
        if (vkCode == L"up") return VK_UP;
        if (vkCode == L"down") return VK_DOWN;
        if (vkCode == L"left") return VK_LEFT;
        if (vkCode == L"right") return VK_RIGHT;
        // 5. 控制与编辑键
        if (vkCode == L"enter") return VK_RETURN;
        if (vkCode == L"esc") return VK_ESCAPE;
        if (vkCode == L"space") return VK_SPACE;
        if (vkCode == L"tab") return VK_TAB;
        if (vkCode == L"backspace") return VK_BACK;
        if (vkCode == L"delete") return VK_DELETE;
        if (vkCode == L"insert") return VK_INSERT;
        if (vkCode == L"home") return VK_HOME;
        if (vkCode == L"end") return VK_END;
        if (vkCode == L"pageup") return VK_PRIOR;
        if (vkCode == L"pagedown") return VK_NEXT;
        if (vkCode == L"printscreen") return VK_SNAPSHOT;
        if (vkCode == L"scrolllock") return VK_SCROLL;
        if (vkCode == L"pause") return VK_PAUSE;
        // 6. 小键盘符号
        if (vkCode == L"numlock") return VK_NUMLOCK;
        if (vkCode == L"*") return VK_MULTIPLY;
        if (vkCode == L"+") return VK_ADD;
        if (vkCode == L"-") return VK_SUBTRACT;
        if (vkCode == L"/") return VK_DIVIDE;
        if (vkCode == L".") return VK_DECIMAL;
        // 7. 主键盘符号键 (OEM Keys)
        // 注意：这些 VK_OEM 键在不同键盘布局下对应的物理按键可能不同
        // 这里以标准美式键盘 (US QWERTY) 为基准进行映射
        if (vkCode == L"`" || vkCode == L"~") return VK_OEM_3;      // ` ~
        if (vkCode == L"-") return VK_OEM_MINUS;                       // - _ (注意：这里会覆盖小键盘的 VK_SUBTRACT，如果需要区分，建议小键盘用 "num-" 或 "numpad-")
        if (vkCode == L"=" || vkCode == L"+") return VK_OEM_PLUS;   // = +
        if (vkCode == L"[") return VK_OEM_4;                           // [ {
        if (vkCode == L"]") return VK_OEM_6;                           // ] }
        if (vkCode == L"\\") return VK_OEM_5;                          // \ |
        if (vkCode == L";") return VK_OEM_1;                           // ; :
        if (vkCode == L"'") return VK_OEM_7;                           // ' "
        if (vkCode == L",") return VK_OEM_COMMA;                       // , <
        if (vkCode == L".") return VK_OEM_PERIOD;                      // . >
        if (vkCode == L"/") return VK_OEM_2;                           // / ?

        // 8. 动态单字符回退 (处理未知的单字符符号)
        // 如果上面都没匹配到，且长度仅为 1，尝试让 Windows 根据当前键盘布局反向解析
        if (vkCode.length() == 1) {
            SHORT vkResult = VkKeyScanW(vkCode[0]);
            if (vkResult != -1) {
                // VkKeyScanW 返回值的低 8 位是虚拟键码，高 8 位是 Shift/Ctrl/Alt 状态
                // 因为我们只需要基础键码，所以取低 8 位即可
                return static_cast<UINT>(vkResult & 0xFF);
            }
        }
        return 0;
    }
    std::wstring Util::getSysLang()
    {
        LANGID langID = GetUserDefaultLangID(); //默认语言标识符
        WCHAR langName[LOCALE_NAME_MAX_LENGTH];
        auto areaId = MAKELCID(langID, SORT_DEFAULT); //区域设置ID
        if (LCIDToLocaleName(areaId, langName, LOCALE_NAME_MAX_LENGTH, 0)) {
            return std::wstring{ langName };
        }
        return L"zh-CN";
    }
    std::wstring Util::readFile(const std::wstring& path)
    {
        HANDLE hFile = CreateFile(path.data(), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
        if (hFile == INVALID_HANDLE_VALUE) return L"";
        LARGE_INTEGER fileSize;
        if (!GetFileSizeEx(hFile, &fileSize)) {
            CloseHandle(hFile);
            return L"";
        }
        if (fileSize.QuadPart == 0) {
            CloseHandle(hFile);
            return L"";
        }
        std::wstring outContent;
        outContent.resize(static_cast<size_t>(fileSize.QuadPart));
        DWORD bytesRead = 0;
        BOOL success = ReadFile(hFile, outContent.data(), static_cast<DWORD>(fileSize.QuadPart), &bytesRead, NULL);
        if (!success) {
            CloseHandle(hFile);
            return L"";
        }
        outContent.resize(bytesRead);
        CloseHandle(hFile);
        if (outContent.size() >= 1 && outContent[0] == 0xFEFF) {
            outContent.erase(0, 1); // 删除第一个字符
        }
        return outContent;
    }
    void Util::saveFile(const std::wstring& path, const std::wstring& content)
    {
        HANDLE hFile = CreateFile(path.data(), GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
        if (hFile == INVALID_HANDLE_VALUE) return;
        DWORD bytesWritten = 0;
        WORD bom = 0xFEFF;
        BOOL success = WriteFile(hFile, &bom, sizeof(WORD), &bytesWritten, NULL);
        if (!success) {
            CloseHandle(hFile); // 出错也要关闭句柄
            return;
        }
        DWORD contentLength = static_cast<DWORD>(content.length() * sizeof(wchar_t));
        success = WriteFile(hFile, content.data(), contentLength, &bytesWritten, NULL);
        if (!success) {
            CloseHandle(hFile);
            return;
        }
        CloseHandle(hFile);
    }
}

