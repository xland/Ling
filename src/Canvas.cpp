#include "pch.h"
#include "../include/Canvas.h"
#include "../include/WinBase.h"
#include "../include/D2D.h"

namespace Ling {

    Canvas::Canvas(WinBase* win) : Node(win)
    {
    }

    Canvas::~Canvas()
    {
        if (painting) finishPaint();
        // 位图是 ctx 的当前 target，ctx 不解绑的话 swap chain 的后台缓冲释放不干净
        if (ctx && targetBmp) ctx->SetTarget(nullptr);
    }

    void Canvas::enableSwapChain()
    {
        if (surface || swap) return;   // 已经建过后端了，改不了
        useSwapChain = true;
    }

    bool Canvas::ensureSwapChain(UINT pxW, UINT pxH)
    {
        auto d2d = D2D::get();
        if (!ctx) {
            // 自己一份 context：它要长期把后台缓冲设为 target，而 d2d->deviceContext 的 target
            // 会被别处（Image / Text / 使用者代码）换掉，共用就会互相打断 BeginDraw。
            // 同一个 device 上开多个 context 是允许的，画刷也能跨 context 复用。
            if (FAILED(d2d->d2dDevice->CreateDeviceContext(D2D1_DEVICE_CONTEXT_OPTIONS_NONE, ctx.ReleaseAndGetAddressOf()))) return false;
        }
        if (swap) {
            DXGI_SWAP_CHAIN_DESC1 desc{};
            swap->GetDesc1(&desc);
            if (desc.Width == pxW && desc.Height == pxH) return true;
            // 换尺寸：先撤掉所有对后台缓冲的引用，ResizeBuffers 才会成功
            ctx->SetTarget(nullptr);
            targetBmp.Reset();
            if (FAILED(swap->ResizeBuffers(0, pxW, pxH, DXGI_FORMAT_UNKNOWN, 0))) return false;
        }
        else {
            swap = d2d->createSwapChain(pxW, pxH);
            if (!swap) return false;
            auto s = d2d->createSurfaceForSwapChain(win->compositor, swap.Get());
            if (!s) { swap.Reset(); return false; }
            auto brush = win->compositor.CreateSurfaceBrush(s);
            brush.Stretch(winrt::Windows::UI::Composition::CompositionStretch::None);
            visual.Brush(brush);
        }
        // 把后台缓冲包成 D2D 位图当绘制目标。CANNOT_DRAW 表示它只能写不能当 source 读，
        // 这正是 swap chain 后台缓冲的语义。
        ComPtr<IDXGISurface> back;
        if (FAILED(swap->GetBuffer(0, IID_PPV_ARGS(back.GetAddressOf())))) return false;
        D2D1_BITMAP_PROPERTIES1 bp{
            .pixelFormat{ D2D1::PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM, D2D1_ALPHA_MODE_PREMULTIPLIED) },
            .dpiX{ 96.f }, .dpiY{ 96.f },
            .bitmapOptions{ D2D1_BITMAP_OPTIONS_TARGET | D2D1_BITMAP_OPTIONS_CANNOT_DRAW }
        };
        if (FAILED(ctx->CreateBitmapFromDxgiSurface(back.Get(), &bp, targetBmp.ReleaseAndGetAddressOf()))) return false;
        ctx->SetTarget(targetBmp.Get());
        return true;
    }

    ID2D1DeviceContext* Canvas::startPaint()
    {
        if (painting) return nullptr;
        const int pxW = static_cast<int>(w);
        const int pxH = static_cast<int>(h);
        if (pxW <= 0 || pxH <= 0) return nullptr;
        if (useSwapChain) {
            if (!ensureSwapChain((UINT)pxW, (UINT)pxH)) return nullptr;
            ctx->BeginDraw();
            // 后台缓冲的原点就是画布原点，不像 DrawingSurface 那样要偏移到 tile 里
            ctx->SetTransform(D2D1::Matrix3x2F::Identity());
            painting = true;
            return ctx.Get();
        }
        if (!surface) {
            auto d2d = D2D::get();
            surface = d2d->createDrawingSurface(win->compositor, (float)pxW, (float)pxH);
            auto brush = win->compositor.CreateSurfaceBrush(surface);
            brush.Stretch(winrt::Windows::UI::Composition::CompositionStretch::None);
            visual.Brush(brush);
        }
        else {
            auto sz = surface.SizeInt32();
            if (sz.Width != pxW || sz.Height != pxH) {
                surface.Resize({ pxW, pxH });
            }
        }
        auto s = surface.as<ABI::Windows::UI::Composition::ICompositionDrawingSurfaceInterop>();
        POINT offset{};
        HRESULT hr = s->BeginDraw(nullptr, __uuidof(ID2D1DeviceContext), reinterpret_cast<void**>(ctx.ReleaseAndGetAddressOf()), &offset);
        if (FAILED(hr)) return nullptr;
        ctx->SetTransform(D2D1::Matrix3x2F::Translation((float)offset.x, (float)offset.y));
        painting = true;
        return ctx.Get();
    }

    void Canvas::finishPaint()
    {
        if (!painting) return;
        painting = false;
        if (useSwapChain) {
            ctx->EndDraw();
            // Present 之后合成器读的才是这一帧。0 = 不等垂直同步，交给 DWM 自己节流
            swap->Present(0, 0);
            // 注意：ctx / targetBmp 保留不释放，下一帧继续用同一条 swap chain
            return;
        }
        auto s = surface.as<ABI::Windows::UI::Composition::ICompositionDrawingSurfaceInterop>();
        s->EndDraw();
        ctx.Reset();
    }
}
