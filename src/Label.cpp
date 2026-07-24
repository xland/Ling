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
		// 契约：measureFunc 必须是纯函数 —— 只读当前节点的度量信息并返回尺寸，
		// 不得修改 style、不得创建/销毁 Composition 资源。否则会在同一次
		// CalculateLayout 内触发 yoga 的重测量循环上限，导致后续兄弟节点的布局
		// 输出为"未完成的中间状态"，视觉上表现为最后一条节点被截掉一半。
		auto self = static_cast<Label*>(YGNodeGetContext(node));
		DWRITE_TEXT_METRICS metrics;
		self->textLayout->GetMetrics(&metrics);
		// 向上取整到整像素，返回给 yoga 用于布局（下游会得到整数 x/y）。
		return {
			std::ceil(metrics.width),
			std::ceil(metrics.height)
		};
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
		// 文字变化后 measureFunc 报告的尺寸也会变，通知 yoga 重新 measure。
		// 只有已经进入 yoga 布局树的节点才能 mark dirty（否则 API 会断言）。
		if (YGNodeGetOwner(node)) YGNodeMarkDirty(node);
		if (surface) paint();
	}

	void Label::setFontSize(float val)
	{
		this->fontSize = val;
		if (textLayout.Get()) {
			textLayout->SetFontSize(fontSize * win->dpi, { 0, INT_MAX });
		}
		if (YGNodeGetOwner(node)) YGNodeMarkDirty(node);
		if (surface) paint();
	}

	void Label::setFontFamily(const std::wstring& val)
	{
		this->fontFamily = val;
		if (textLayout.Get() && !fontFamily.empty()) {
			textLayout->SetFontFamilyName(fontFamily.data(), { 0, INT_MAX });
		}
		if (YGNodeGetOwner(node)) YGNodeMarkDirty(node);
		if (surface) paint();
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
		// surface 的尺寸随 layout 出的 w/h 走：yoga 会先调 measureFunc 得到尺寸、
		// 再走完整个布局，然后调用 Node::layout() → 我们的 layout() → paint()。
		// 此刻 w/h 已经是最终整数像素，一次到位创建/resize。
		const int pxW = static_cast<int>(w);
		const int pxH = static_cast<int>(h);
		if (pxW <= 0 || pxH <= 0) return;
		if (!surface) {
			auto d2d = D2D::get();
			surface = d2d->createDrawingSurface(win->compositor, pxW, pxH);
			Composition::CompositionSurfaceBrush brush = win->compositor.CreateSurfaceBrush(surface);
			visual.Brush(brush);
		}
		else {
			// 只有尺寸变了才 Resize，避免每帧重建
			auto sz = surface.SizeInt32();
			if (sz.Width != pxW || sz.Height != pxH) {
				surface.Resize({ pxW, pxH });
			}
		}

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
