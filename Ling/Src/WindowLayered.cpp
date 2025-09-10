#include <dwmapi.h>
#include <thorvg.h>
#include "../Include/App.h"
#include "../Include/WindowLayered.h"

namespace Ling {
    void WindowLayered::createNativeWindow()
    {
        scaledCanvas = tvg::SwCanvas::gen();
        auto pos = getWindowPosition();
        auto size = getWindowSize();
        hwnd = CreateWindowEx(WS_EX_LAYERED, getWinClsName().data(), title.data(), WS_POPUP,
            pos.x, pos.y, size.w, size.h, nullptr, nullptr, App::get()->hInstance, nullptr);
        SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(this));
        setScaleFactor();
        resetCanvas();
    }
    void WindowLayered::show() {
        repaint();
        WindowBase::show();
    }
    void WindowLayered::repaint()
    {
        canvas->update();
        canvas->draw();
        canvas->sync();
        scalePix();
        auto size = getWindowClientSize();
        HDC hdc = GetDC(hwnd);
        auto compDC = CreateCompatibleDC(hdc);
        auto bitmap = CreateCompatibleBitmap(hdc, size.w, size.h);
        DeleteObject(SelectObject(compDC, bitmap));
        BITMAPINFO info = { sizeof(BITMAPINFOHEADER), size.w, -size.h, 1, 32, BI_RGB, size.w * 4 * size.h, 0, 0, 0, 0 };
        SetDIBits(hdc, bitmap, 0, size.h, scaledBuffer.data(), &info, DIB_RGB_COLORS);
        BLENDFUNCTION blend = { .BlendOp{AC_SRC_OVER}, .SourceConstantAlpha{255}, .AlphaFormat{AC_SRC_ALPHA} };
        POINT pSrc = { 0, 0 };
        SIZE sizeWnd = { size.w, size.h };
        UpdateLayeredWindow(hwnd, hdc, NULL, &sizeWnd, compDC, &pSrc, NULL, &blend, ULW_ALPHA);
        ReleaseDC(hwnd, hdc);
        DeleteDC(compDC);
        DeleteObject(bitmap);        
    }

    LRESULT WindowLayered::customMsgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
    {
        switch (msg)
        {
        case WM_NCHITTEST:
        {
            int x = GET_X_LPARAM(lParam);
            int y = GET_Y_LPARAM(lParam);
            return this->hitTest(x, y);
            break;
        }
        }
        return WindowBase::customMsgProc(hwnd, msg, wParam, lParam);
    }

    LRESULT WindowLayered::hitTest(int x, int y) {
        if (!hwnd) return HTNOWHERE;
        RECT winRect;
        GetWindowRect(hwnd, &winRect);
        if (x > winRect.left && y > winRect.top && x < winRect.right && y < winRect.bottom) {
            if (resizable) {
                int borderWidth = 5;
                if (x < winRect.left + borderWidth && y < winRect.top + borderWidth) return HTTOPLEFT;
                else if (x < winRect.left + borderWidth && y > winRect.bottom - borderWidth) return HTBOTTOMLEFT;
                else if (x > winRect.right - borderWidth && y > winRect.bottom - borderWidth) return HTBOTTOMRIGHT;
                else if (x > winRect.right - borderWidth && y < winRect.top + borderWidth) return HTTOPRIGHT;
                else if (x < winRect.left + borderWidth) return HTLEFT;
                else if (x > winRect.right - borderWidth) return HTRIGHT;
                else if (y < winRect.top + borderWidth) return HTTOP;
                else if (y > winRect.bottom - borderWidth) return HTBOTTOM;
            }
            auto ele = getElementByPosition(x - winRect.left, y - winRect.top);
            if (ele->getCaptionFlag()) {
                return HTCAPTION;
            }
            return HTCLIENT;
        }
        else
        {
            return HTNOWHERE;
        }
    }
    void WindowLayered::scalePix()
    {
        auto size = getWindowClientSize();
        int w = size.w * scaleFactor;
        int h = size.h * scaleFactor;
        if (scaledBuffer.size() != size.w * size.h) 
        {
            scaledBuffer.resize(size.w * size.h);
            scaledBuffer.shrink_to_fit();
        }
        scaledCanvas->target(scaledBuffer.data(), size.w, size.w, size.h, tvg::ColorSpace::ARGB8888);
        auto picture = tvg::Picture::gen();
        picture->load(buffer.data(), w, h, tvg::ColorSpace::ARGB8888);
        picture->scale(1/scaleFactor);
        scaledCanvas->push(std::move(picture));
        scaledCanvas->draw();
        scaledCanvas->sync();
    }
}