#pragma once
#include <string>
#include <wrl.h>
#include <dwrite_3.h>
#include <yoga/Yoga.h>
#include <winrt/Windows.UI.Composition.h>
#include "Element.h"
namespace Ling{
	using namespace Microsoft::WRL;
	class WindowBase;
	class LabelStyle;
	class Label : public Element
	{
		friend class LabelStyle;
	public:
		Label(WindowBase* win);
		virtual ~Label();
		std::shared_ptr<LabelStyle> createStyle();
		void setText(const std::wstring& text);
	protected:
		void layout() override;
	private:
		void setForegroundColor(const Color& color);
		void setForegroundHoverColor(const Color& color);
		void setFontSize(const float& fontSize);
		void setFontFamily(const std::wstring& fontFamily);
		static YGSize nodeMeasureCB(YGNodeConstRef node, float width, YGMeasureMode widthMode, float height, YGMeasureMode heightMode);
		void paint();
	private:
		std::wstring text;
		ComPtr<ID2D1SolidColorBrush> foreColorBrush;
		ComPtr<ID2D1SolidColorBrush> foreHoverColorBrush;
		Composition::SpriteVisual textVisual{ nullptr };
		YGNodeRef textNode{ nullptr };
		ComPtr<IDWriteTextLayout> textLayout;
		Composition::CompositionSurfaceBrush brush{nullptr};
		Composition::CompositionDrawingSurface surface{ nullptr };
	};
}

