#include <Windows.h>
#include <thorvg.h>
#include "../Include/WindowBase.h"

namespace Ling {

    LRESULT CALLBACK WindowBase::routeWindowMsg(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
        auto winObj = reinterpret_cast<WindowBase*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
        if (winObj)
        {
            return winObj->windowMsgProc(hwnd, msg, wParam, lParam);
        }
        else {
            return DefWindowProc(hwnd, msg, wParam, lParam);
        }
    }
    LRESULT CALLBACK WindowBase::windowMsgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
    {
        switch (msg)
        {
            case WM_CREATE:
            {
                auto a = 1;
                return 0;
            }
            case WM_NCDESTROY:
            {
                SetWindowLongPtr(hwnd, GWLP_USERDATA, 0);
                PostQuitMessage(0);
                return 0;
            }
            case WM_DESTROY:
            {
                SetWindowLongPtr(hwnd, GWLP_USERDATA, 0);
                PostQuitMessage(0);
                return 0;
            }
            case WM_MOVE:
            {
                setWindowPosition(LOWORD(lParam), HIWORD(lParam));
                return 0;
            }
            case WM_SIZE:
            {
                int w{ LOWORD(lParam) }, h{ HIWORD(lParam) };
                setSize(w, h);
                if (w == 0 || h == 0) {
                    return 0;
                }
                if (isWinInit) {
                    isWinInit = false;
                    return 0;
                }
                resetCanvas();
                layout();
                return 0;
            }
            case WM_DPICHANGED:
            {
                scaleFactor = LOWORD(wParam) / 96.0f;
                dpiChanged(reinterpret_cast<RECT*>(lParam));
                return 0;
            }
            case WM_MOUSEMOVE:
            {
                windowMouseMove(LOWORD(lParam), HIWORD(lParam));
                return 0;
            }
            case WM_SETCURSOR: {
                if (LOWORD(lParam) == HTCLIENT)
                {
                    return 1;
                }
                else
                {
                    hoverEle = nullptr;
                    break;
                }
            }
            case WM_ERASEBKGND:
            {
                return 1;
            }
            case WM_TIMER: {
                if (wParam == FlashCaretTimer) {
                    InvalidateRect(hwnd, nullptr, false);
                }
                return 0;
            }
            case WM_PAINT: {
                paintArea();
                return 0;
            }
            case WM_LBUTTONDBLCLK:
            {
                return 0;
            }
            case WM_LBUTTONDOWN:
            {
                windowMouseDown(LOWORD(lParam), HIWORD(lParam), MouseButton::Left);
                return 0;
            }
            case WM_LBUTTONUP:
            {
                windowMouseUp(LOWORD(lParam), HIWORD(lParam), MouseButton::Left);
                return 0;
            }
            case WM_RBUTTONDOWN:
            {
                windowMouseDown(LOWORD(lParam), HIWORD(lParam), MouseButton::Right);
                return 0;
            }
            case WM_RBUTTONUP:
            {
                windowMouseUp(LOWORD(lParam), HIWORD(lParam), MouseButton::Right);
                return 0;
            }
            case WM_KEYDOWN:
            {
                return windowKeyDown(wParam);
            }
            case WM_KEYUP:
            {
                return windowKeyUp(wParam);
            }
            case WM_CHAR:
            {
                return 1;
            }
            case WM_IME_STARTCOMPOSITION:
            {
                break;
            }
            case WM_IME_ENDCOMPOSITION:
            {
                break;
            }
            case WM_IME_NOTIFY:
            {
                switch (wParam)
                {
                case IMN_SETOPENSTATUS:
                    break;
                default:
                    break;
                }
                break;
            }
        }
        return customMsgProc(hwnd, msg, wParam, lParam);
    }

    LRESULT CALLBACK WindowBase::customMsgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
    {
        return DefWindowProc(hwnd, msg, wParam, lParam);
    }
    void WindowBase::dpiChanged(RECT* suggestedRect)
    {
        scene->scale(scaleFactor);
        for (const auto& pair : dpiChangedCBs) {
            pair.second();
        }
        auto w{ suggestedRect->right - suggestedRect->left };
        auto h{ suggestedRect->bottom - suggestedRect->top };
        SetWindowPos(hwnd, nullptr, suggestedRect->left, suggestedRect->top, w, h, SWP_NOZORDER | SWP_NOACTIVATE);
    }
    void WindowBase::paintArea()
    {
        //auto text = tvg::Text::gen();    
        //std::u8string str = u8R"(醉里挑灯看剑，梦回吹角连营。Abc, Def)";
        //text->text(reinterpret_cast<const char*>(str.c_str()));
        //text->font("SimHei");
        //text->size(21.f);
        //text->fill(0, 0, 0);
        //float x1, y1, w1, h1;
        //text->bounds(&x1, &y1, &w1, &h1);
        //auto ss = getWindowClientSize();
        //text->translate(ss.w-w1-x1, ss.h-h1-y1);
        //scene->push(text);


        canvas->update();
        canvas->draw();
        canvas->sync();
        auto size = getWindowClientSize();
        auto w = size.w * scaleFactor;
        auto h = size.h * scaleFactor;
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hwnd, &ps);
        BITMAPINFO bmi{};
        bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
        bmi.bmiHeader.biWidth = w;
        bmi.bmiHeader.biHeight = -h;  // top-down
        bmi.bmiHeader.biPlanes = 1;
        bmi.bmiHeader.biBitCount = 32;
        bmi.bmiHeader.biCompression = BI_RGB;
        SetStretchBltMode(hdc, HALFTONE);
        SetBrushOrgEx(hdc, 0, 0, nullptr);
        StretchDIBits(hdc, 0, 0, size.w, size.h, 0, 0, w, h,
            buffer.data(), &bmi, DIB_RGB_COLORS, SRCCOPY);
        EndPaint(hwnd, &ps);
    }
    int WindowBase::windowKeyDown(const int& key)
    {
        switch (key)
        {
            case VK_DELETE: {
                return 0;
            }
            case VK_LEFT: {
                return 0;
            }
            case VK_RIGHT: {
                return 0;
            }
            case VK_ESCAPE: {
                return 0;
            }
            case VK_SHIFT: {
                return 0;
            }
            case VK_CONTROL: {
                return 0;
            }
        }
        return 0;
    }
    int WindowBase::windowKeyUp(const int& key) {
        switch (key)
        {
            case VK_SHIFT: {
                return 0;
            }
            case VK_CONTROL: {
                return 0;
        }
        }
        return 0;
    }
}