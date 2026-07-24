#include "pch.h"
#include "../include/Event.h"
#include "../include/Label.h"
#include "../include/WinBase.h"
#include "../include/D2D.h"

namespace Ling {

	Label::Label(WinBase* win) :Node(win)
	{
		YGNodeSetContext(this->node, this);
		YGNodeSetMeasureFunc(this->node, &Label::nodeMeasureCB);
	}

	Label::~Label()
	{
	}

	YGSize Label::nodeMeasureCB(YGNodeConstRef node, float width, YGMeasureMode widthMode, float height, YGMeasureMode heightMode)
	{
		//如果你在 YGNodeStyleSetWidth(node, 100) / YGNodeStyleSetHeight(node, 50) 里已经指定了固定大小，Yoga 就直接用这个值，不会去调用 measureFunc。
		//如果父容器已经约束住了大小，比如 flex : 1 填充满了，Yoga 也不会再问 measureFunc。
		auto self = static_cast<Label*>(YGNodeGetContext(node));
		DWRITE_TEXT_METRICS metrics;
		self->textLayout->GetMetrics(&metrics);


		const int pxW = static_cast<int>(std::ceil(metrics.width));
		const int pxH = static_cast<int>(std::ceil(metrics.height));
		YGNodeStyleSetWidth(self->node, static_cast<float>(pxW));
		YGNodeStyleSetHeight(self->node, static_cast<float>(pxH));
		if (!self->surface) {
			auto d2d = D2D::get();
			self->surface = d2d->createDrawingSurface(self->win->compositor, pxW, pxH);
			Composition::CompositionSurfaceBrush brush = self->win->compositor.CreateSurfaceBrush(self->surface);
			self->visual.Brush(brush);
		}
		else {
			self->surface.Resize({ pxW, pxH });
		}


		return { metrics.width, metrics.height };
	}

	void Label::setText(const std::wstring& text)
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

	void Label::setFontSize(float val)
	{
		this->fontSize = val;
		if (textLayout.Get()) {
			textLayout->SetFontSize(fontSize * win->dpi, { 0, INT_MAX });
		}
		if (surface) {
			paint();
		}
	}

	void Label::setFontFamily(const std::wstring& val)
	{
		this->fontFamily = val;
		if (textLayout.Get() && !fontFamily.empty()) {
			textLayout->SetFontFamilyName(fontFamily.data(), { 0, INT_MAX });
		}
		if (surface) {
			paint();
		}
	}

	void Label::setColor(Color color)
	{
		if (this->color == color)  return;
		this->color = color;
		if (surface) {
			paint();
		}
	}

	void Label::paint()
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

	void Label::layout()
	{
		Node::layout(); 
		paint();
	}
}
