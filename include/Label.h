#pragma once
#include <wrl.h>
#include <winrt/Windows.UI.Composition.h>
#include "Node.h"

namespace Ling {
	class WinBase;
	class Label :public Node
	{
	public:
		Label(WinBase* win);
		~Label();
		void setText(const std::wstring& text);
		void setFontSize(float val);
		void setFontFamily(const std::wstring& val);
		void setColor(Color color);
		void setBg(const Color& color) override;
		void paint();
	public:
		winrt::Windows::UI::Composition::CompositionDrawingSurface surface{ nullptr };
		Microsoft::WRL::ComPtr<IDWriteTextLayout> textLayout;
	private:
		using Node::makeChild;
		using Node::setFlexDirection;
		static YGSize nodeMeasureCB(YGNodeConstRef node, float width, YGMeasureMode widthMode, float height, YGMeasureMode heightMode);
		void layout() override;
	private:
		Color color{ 0x333333FF };
		std::wstring text;
		std::wstring fontFamily;
		float fontSize{ 12.f }, metricW, metricH;
	};
}
