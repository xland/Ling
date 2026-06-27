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
	class Text : public Element<Text>
	{
	public:
		Text(WindowBase* win);
		~Text();
		Text* setText(const std::wstring text);
		Text* setFontSize(const float& fontSize);
		Text* setForeColor(const Color& color);
		Text* setFontFamily(const std::wstring& fontFamily);
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

