#include "pch.h"
#include "Label.h"
#include "D2D.h"
#include "WindowBase.h"
#include "LabelStyle.h"
namespace Ling {
	Label::Label(WindowBase* win):Element(win), textNode(YGNodeNew()), textVisual{ win->compositor.CreateSpriteVisual() }
	{
		visual.Children().InsertAtTop(textVisual);
		YGNodeInsertChild(node, textNode, YGNodeGetChildCount(node));
		YGNodeSetContext(textNode, this);
	}

	Label::~Label()
	{
	}

	std::shared_ptr<LabelStyle> Label::createStyle()
	{
		auto s = std::shared_ptr<LabelStyle>(new LabelStyle());
		this->style = s;
		s->elements.push_back(this);
		return s;
	}

	void Label::setText(const std::wstring& text)
	{
		auto d2d = D2D::get();
		surface = d2d->createDrawingSurface(win->compositor);
		brush = win->compositor.CreateSurfaceBrush(surface);
		textVisual.Brush(brush);
		this->text = text;
		textLayout = d2d->createTextLayout(text.data(), FLT_MAX, FLT_MAX);
		YGNodeSetMeasureFunc(textNode, &Label::nodeMeasureCB);
		if (style.get()) {
			auto subStyle = std::dynamic_pointer_cast<LabelStyle>(style);
			textLayout->SetFontFamilyName(subStyle->getFontFamily().data(), { 0,MAXINT });
			textLayout->SetFontSize(subStyle->getFontSize() * win->dpi, { 0,MAXINT });
		}
	}

	void Label::layout()
	{
		x = YGNodeLayoutGetLeft(textNode);
		y = YGNodeLayoutGetTop(textNode);
		xAbs = parent->xAbs + x;
		yAbs = parent->yAbs + y;
		w = YGNodeLayoutGetWidth(textNode);
		h = YGNodeLayoutGetHeight(textNode);
		textVisual.Offset({ x, y, 0.0f });
		textVisual.Size({ w, h });
		surface.Resize({(int)w,(int)h});
		paint();
	}
	void Label::setForegroundColor(const Color& color)
	{
		foreColorBrush.Reset();
	}
	void Label::setForegroundHoverColor(const Color& color)
	{
		foreHoverColorBrush.Reset();
	}
	void Label::setFontSize(const float& fontSize)
	{
		if (textLayout.Get()) {
			textLayout->SetFontSize(fontSize * win->dpi, { 0,MAXINT });
		}
	}
	void Label::setFontFamily(const std::wstring& fontFamily)
	{
		if (textLayout.Get()) {
			textLayout->SetFontFamilyName(fontFamily.data(), { 0,MAXINT });
		}
	}
	YGSize Label::nodeMeasureCB(YGNodeConstRef node, float width, YGMeasureMode widthMode, float height, YGMeasureMode heightMode)
	{
		//如果你在 YGNodeStyleSetWidth(node, 100) / YGNodeStyleSetHeight(node, 50) 里已经指定了固定大小，Yoga 就直接用这个值，不会去调用 measureFunc。
		//如果父容器已经约束住了大小，比如 flex : 1 填充满了，Yoga 也不会再问 measureFunc。
		auto self = static_cast<Label*>(YGNodeGetContext(node));
		float maxWidth = (widthMode == YGMeasureModeUndefined) ? 0.0f : width;
		DWRITE_TEXT_METRICS metrics;
		self->textLayout->GetMetrics(&metrics);
		return { metrics.width, metrics.height };
	}
	void Label::paint()
	{
		auto surfaceInterop = surface.as<ABI::Windows::UI::Composition::ICompositionDrawingSurfaceInterop>();
		ComPtr<ID2D1DeviceContext> d2dContext;
		POINT offset{};
		HRESULT hr = surfaceInterop->BeginDraw(nullptr, __uuidof(ID2D1DeviceContext), reinterpret_cast<void**>(d2dContext.GetAddressOf()), &offset);
		auto trans = D2D1::Matrix3x2F::Translation(static_cast<float>(offset.x), static_cast<float>(offset.y));
		d2dContext->SetTransform(trans);
		d2dContext->Clear(0);
		if (!foreColorBrush.Get()) {
			foreColorBrush.Reset();
			auto subStyle = std::dynamic_pointer_cast<LabelStyle>(style);
			auto color = subStyle->getForegroundColor().getD2DColor();
			d2dContext->CreateSolidColorBrush(color, foreColorBrush.GetAddressOf());
		}		
		d2dContext->DrawTextLayout({ 0, 0 }, textLayout.Get(), foreColorBrush.Get());
		surfaceInterop->EndDraw();
	}
}