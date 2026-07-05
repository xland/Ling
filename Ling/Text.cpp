#include "pch.h"
#include "Text.h"
#include "D2D.h"
#include "WindowBase.h"
#include "Property.h"
namespace Ling {
	Text::Text(WindowBase* win):Element(win)
	{
	}

	Text::~Text()
	{
	}

	void Text::propertyChanged(const Ling::PropertyType& key, const void* value)
	{
		Element::propertyChanged(key, value);
		if (key == PropertyType::Text) {
			auto d2d = D2D::get();
			surface = d2d->createDrawingSurface(win->compositor);
			brush = win->compositor.CreateSurfaceBrush(surface);
			visual.Brush(brush);
			auto str = (std::wstring*)value;
			textLayout = d2d->createTextLayout(str->data(), FLT_MAX, FLT_MAX);
			if (property->hasFontFamily()) {
				textLayout->SetFontFamilyName(property->getFontFamily().data(), {0,MAXINT});
			}
			if (property->hasFontSize()) {
				textLayout->SetFontSize(property->getFontSize() * win->dpi, { 0,MAXINT });
			}
			YGNodeSetMeasureFunc(node, &Text::nodeMeasureCB);
		}
		else if (key == PropertyType::FontSize) {
			if (textLayout.Get()) {
				textLayout->SetFontSize(*(float*)value * win->dpi, { 0,MAXINT });
			}
		}
		else if (key == PropertyType::FontFamily) {
			if (textLayout.Get()) {
				textLayout->SetFontFamilyName(((std::wstring*)value)->data(), { 0,MAXINT });
			}
		}
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
		paint();
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
	void Text::paint()
	{
		auto surfaceInterop = surface.as<ABI::Windows::UI::Composition::ICompositionDrawingSurfaceInterop>();
		ComPtr<ID2D1DeviceContext> d2dContext;
		POINT offset{};
		HRESULT hr = surfaceInterop->BeginDraw(nullptr, __uuidof(ID2D1DeviceContext), reinterpret_cast<void**>(d2dContext.GetAddressOf()), &offset);
		auto trans = D2D1::Matrix3x2F::Translation(static_cast<float>(offset.x), static_cast<float>(offset.y));
		d2dContext->SetTransform(trans);
		d2dContext->Clear(0);
		ComPtr<ID2D1SolidColorBrush> brush;
		auto color = property->getColorForeground().getD2DColor();
		d2dContext->CreateSolidColorBrush(color, brush.GetAddressOf());
		d2dContext->DrawTextLayout({ 0, 0 }, textLayout.Get(), brush.Get());
		surfaceInterop->EndDraw();
	}
}