#include "pch.h"
#include "../include/D2D.h"
#include "../include/WinBase.h"
#include "../include/Slider.h"
namespace Ling {
	Slider::Slider(WinBase* win) : Node(win)
	{
	}
	Slider::~Slider()
	{
	}
	void Slider::layout()
	{
		Node::layout();
        const int pxW = static_cast<int>(w);
        const int pxH = static_cast<int>(h);
        if (pxW <= 0 || pxH <= 0) return;
        if (!surface) {
            auto d2d = D2D::get();
            surface = d2d->createDrawingSurface(win->compositor, (float)pxW, (float)pxH);
            auto brush = win->compositor.CreateSurfaceBrush(surface);
            visual.Brush(brush);
        }
        else {
            auto sz = surface.SizeInt32();
            if (sz.Width != pxW || sz.Height != pxH) {
                surface.Resize({ pxW, pxH });
            }
        }

        auto s = surface.as<ABI::Windows::UI::Composition::ICompositionDrawingSurfaceInterop>();
        ComPtr<ID2D1DeviceContext> ctx;
        POINT offset{};
        s->BeginDraw(nullptr, __uuidof(ID2D1DeviceContext), reinterpret_cast<void**>(ctx.GetAddressOf()), &offset);
        auto trans = D2D1::Matrix3x2F::Translation((float)offset.x, (float)offset.y);
        ctx->SetTransform(trans);
        ctx->Clear(0);    

        ComPtr<ID2D1SolidColorBrush> lineBrush;
        ctx->CreateSolidColorBrush(Color(0xBBBBBBFF).getD2DColor(), lineBrush.GetAddressOf());
		ctx->DrawLine({ 0.f,h / 2 }, { w, h/2 }, lineBrush.Get(), 2.f);

        s->EndDraw();
	}
}