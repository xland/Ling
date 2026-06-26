#include "Text.h"
#include "D2D.h"
#include "WindowBase.h"
namespace Ling {
	Text::Text(WindowBase* win):Element<Text>(win)
	{
	}

	Text::~Text()
	{
	}

	void Text::setText(const std::wstring text)
	{
		this->text = text;
	}

	void Text::setFontSize(const float& fontSize)
	{
		this->fontSize = fontSize;
	}

	void Text::setForeColor(const Color& color)
	{
		foreColor = color;
	}

	void Text::setFontFamily(const std::wstring& fontFamily)
	{
		this->fontFamily = fontFamily;
	}

	void Text::initProperty()
	{
		auto d2d = D2D::get();
		surface = d2d->createDrawingSurface(win->compositor);
		brush = win->compositor.CreateSurfaceBrush(surface);
		visual.Brush(brush);
		textLayout = d2d->createTextLayout(text, FLT_MAX, FLT_MAX);
		textLayout->SetFontSize(fontSize * win->dpi, { 0,(unsigned int)text.length() });
		textLayout->SetFontFamilyName(fontFamily.data(), {0,(unsigned int)text.length()});
		YGNodeSetMeasureFunc(node, &Text::nodeMeasureCB);
	}
	void Text::layout()
	{
		x = YGNodeLayoutGetLeft(node);
		y = YGNodeLayoutGetTop(node);
		xAbs = parent->xAbs + x;
		yAbs = parent->yAbs + y;
		w = YGNodeLayoutGetWidth(node);
		h = YGNodeLayoutGetHeight(node);
		visual.Offset({ x, y, 0.0f });
		visual.Size({ w, h });
		surface.Resize({(int)w,(int)h});
		auto surfaceInterop = surface.as<ABI::Windows::UI::Composition::ICompositionDrawingSurfaceInterop>();
		ComPtr<ID2D1DeviceContext> d2dContext;
		POINT offset{};
		HRESULT hr = surfaceInterop->BeginDraw(nullptr,__uuidof(ID2D1DeviceContext),reinterpret_cast<void**>(d2dContext.GetAddressOf()),&offset);
		auto trans = D2D1::Matrix3x2F::Translation(static_cast<float>(offset.x), static_cast<float>(offset.y));
		d2dContext->SetTransform(trans);
		d2dContext->Clear(backgroundColor.getD2DColor());
		ComPtr<ID2D1SolidColorBrush> brush;
		d2dContext->CreateSolidColorBrush(foreColor.getD2DColor(), brush.GetAddressOf());
		d2dContext->DrawTextLayout({ 0, 0 }, textLayout.Get(), brush.Get());
		surfaceInterop->EndDraw();
	}
	YGSize Text::nodeMeasureCB(YGNodeConstRef node, float width, YGMeasureMode widthMode, float height, YGMeasureMode heightMode)
	{
		//如果你在 YGNodeStyleSetWidth(node, 100) / YGNodeStyleSetHeight(node, 50) 里已经指定了固定大小，Yoga 就直接用这个值，不会去调用 measureFunc。
		//如果父容器已经约束住了大小，比如 flex : 1 填充满了，Yoga 也不会再问 measureFunc。
		auto self = static_cast<Text*>(YGNodeGetContext(node));
		float maxWidth = (widthMode == YGMeasureModeUndefined) ? 0.0f : width;
		DWRITE_TEXT_METRICS metrics;
		self->textLayout->GetMetrics(&metrics);
		return { metrics.width, metrics.height };
	}
}