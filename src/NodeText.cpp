#include "pch.h"
#include "../include/Event.h"
#include "../include/NodeText.h"
#include "../include/WinBase.h"
#include "../include/D2D.h"

namespace Ling {

	NodeText::NodeText(WinBase* win) :Node(win)
	{
	}

	NodeText::~NodeText()
	{
	}

	void NodeText::setText(const std::wstring& text)
	{
		this->text = text;
		auto d2d = D2D::get();
		textLayout = d2d->createTextLayout(text, FLT_MAX, FLT_MAX);
		textLayout->SetFontSize(fontSize * win->dpi, { 0, INT_MAX });
		if (!fontFamily.empty()) {
			textLayout->SetFontFamilyName(fontFamily.data(), { 0, INT_MAX });
		}
		if (surface) {
			paint();
		}
	}

	void NodeText::setFontSize(float val)
	{
		this->fontSize = val;
		if (textLayout.Get()) {
			textLayout->SetFontSize(fontSize * win->dpi, { 0, INT_MAX });
		}
		if (surface) {
			paint();
		}
	}

	void NodeText::setFontFamily(const std::wstring& val)
	{
		this->fontFamily = val;
		if (textLayout.Get() && !fontFamily.empty()) {
			textLayout->SetFontFamilyName(fontFamily.data(), { 0, INT_MAX });
		}
		if (surface) {
			paint();
		}
	}

	void NodeText::setColor(Color color)
	{
		if (this->color == color)  return;
		this->color = color;
		if (surface) {
			paint();
		}
	}

	void NodeText::paint()
	{
		auto s = surface.as<ABI::Windows::UI::Composition::ICompositionDrawingSurfaceInterop>();
		ComPtr<ID2D1DeviceContext> ctx;
		POINT offset{};   // 物理像素
		HRESULT hr = s->BeginDraw(nullptr, __uuidof(ID2D1DeviceContext), reinterpret_cast<void**>(ctx.GetAddressOf()), &offset);
		// 全程使用物理像素，不调 SetDpi
		auto trans = D2D1::Matrix3x2F::Translation(static_cast<float>(offset.x), static_cast<float>(offset.y));
		ctx->SetTransform(trans);
		ctx->Clear(0);
		ComPtr<ID2D1SolidColorBrush> brush;
		ctx->CreateSolidColorBrush(color.getD2DColor(), brush.GetAddressOf());
		ctx->DrawTextLayout({ 0.f, 0.f }, textLayout.Get(), brush.Get());
		s->EndDraw();
	}

	void NodeText::layout()
	{
		Node::layout(); 
		paint();
	}

	void NodeText::beforeLayout()
	{
		DWRITE_TEXT_METRICS metrics;
		textLayout->GetMetrics(&metrics);
		// textLayout 里已经是物理像素了，直接设给 yoga，不再走 setSize（那会把它当逻辑值再乘一遍 dpi）
		YGNodeStyleSetWidth(node, metrics.width);
		YGNodeStyleSetHeight(node, metrics.height);
		if (!surface) {
			auto d2d = D2D::get();
			surface = d2d->createDrawingSurface(win->compositor, (int)metrics.width, (int)metrics.height);
			Composition::CompositionSurfaceBrush brush = win->compositor.CreateSurfaceBrush(surface);
			visual.Brush(brush);
		}
		else {
			surface.Resize({ (int)metrics.width, (int)metrics.height });
		}
	}

}
