#include <dwmapi.h>
#include <thorvg.h>
#include "../Include/App.h"
#include "../Include/WindowLayered.h"

namespace Ling {
    void WindowLayered::createNativeWindow()
    {
        auto pos = getWindowPosition();
        auto size = getWindowSize();
        hwnd = CreateWindowEx(WS_EX_LAYERED, getWinClsName().data(), title.data(), WS_POPUP | WS_CLIPCHILDREN | WS_CLIPSIBLINGS,
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

        for (auto& px : buffer)
        {
            uint8_t a = (px >> 24) & 0xFF; // A
            uint8_t r = (px >> 16) & 0xFF; // R
            uint8_t g = (px >> 8) & 0xFF; // G
            uint8_t b = (px) & 0xFF; // B

            // premultiply (带四舍五入，避免暗化)
            uint8_t r_p = static_cast<uint8_t>((r * a + 127) / 255);
            uint8_t g_p = static_cast<uint8_t>((g * a + 127) / 255);
            uint8_t b_p = static_cast<uint8_t>((b * a + 127) / 255);

            // 重新打包成 BGRA
            px = (a << 24) | (r_p << 16) | (g_p << 8) | (b_p);
        }

        auto size = getWindowClientSize(); 
        auto w = size.w * scaleFactor;
        auto h = size.h * scaleFactor;

        auto hdc = GetDC(hwnd);
        auto compDC = CreateCompatibleDC(hdc);

        // 注意：bitmap 的大小是窗口大小，不是画布大小
        auto bitmap = CreateCompatibleBitmap(hdc, size.w, size.h);
        DeleteObject(SelectObject(compDC, bitmap));

        // 设置缩放模式
        SetStretchBltMode(compDC, HALFTONE);
        SetBrushOrgEx(compDC, 0, 0, nullptr);

        BITMAPINFO bmi{};
        bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
        bmi.bmiHeader.biWidth = w;
        bmi.bmiHeader.biHeight = -h; // top-down
        bmi.bmiHeader.biPlanes = 1;
        bmi.bmiHeader.biBitCount = 32;
        bmi.bmiHeader.biCompression = BI_RGB;

        // 这里就把大画布缩小绘制到 compDC 里
        StretchDIBits(compDC,
            0, 0, size.w, size.h,     // 目标窗口大小
            0, 0, w, h,               // 源画布大小
            buffer.data(),
            &bmi,
            DIB_RGB_COLORS,
            SRCCOPY
        );

        // 更新 layered window
        BLENDFUNCTION blend = { AC_SRC_OVER, 0xFF, AC_SRC_ALPHA, 0 };
        POINT pSrc = { 0, 0 };
        SIZE sizeWnd = { size.w, size.h };
        UpdateLayeredWindow(hwnd, hdc, NULL, &sizeWnd, compDC, &pSrc, NULL, &blend, ULW_ALPHA);

        ReleaseDC(hwnd, hdc);
        DeleteDC(compDC);
        DeleteObject(bitmap);
    }
}