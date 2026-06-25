#pragma once
#include "Header.h"
#include "Element.h"
namespace Ling{
	class Text : public Element
	{
	public:
		Text(WindowBase* win);
		~Text();
		void setText(const std::wstring text);
		void setFontSize(const float& fontSize);
		void setForeColor(const Color& color);
		void setFontFamily(const std::wstring& fontFamily);
	protected:
		void initProperty() override;
		void layout() override;
	private:
		static YGSize nodeMeasureCB(YGNodeConstRef node, float width, YGMeasureMode widthMode, float height, YGMeasureMode heightMode);
	private:
		ComPtr<IDWriteTextLayout> textLayout;
		std::wstring text;
		Composition::CompositionSurfaceBrush brush{nullptr};
		Composition::CompositionDrawingSurface surface{ nullptr };
		Color foreColor;
		float fontSize;
		std::wstring fontFamily;
	};
}

