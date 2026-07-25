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
    }

    ID2D1DeviceContext* Canvas::startPaint()
    {
        if (painting) return nullptr;
        const int pxW = static_cast<int>(w);
        const int pxH = static_cast<int>(h);
        if (pxW <= 0 || pxH <= 0) return nullptr;
        if (!surface) {
            auto d2d = D2D::get();
            surface = d2d->createDrawingSurface(win->compositor, (float)pxW, (float)pxH);
            auto brush = win->compositor.CreateSurfaceBrush(surface);
            // surface 尺寸恒等于 visual 尺寸，None 只是防止外部再包一层 visual 时被拉伸。
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
        auto s = surface.as<ABI::Windows::UI::Composition::ICompositionDrawingSurfaceInterop>();
        s->EndDraw();
        ctx.Reset();
        painting = false;
    }
}
