#pragma once
#include <wrl.h>
#include <winrt/Windows.UI.Composition.h>
#include "Node.h"

namespace Ling {
	class WinBase;
	class Button;
	class Label :public Node
	{
		friend class WinBase;
		friend class Button;
	public:
		Label(WinBase* win);
		~Label();
		void setText(const std::wstring& text);
		void setFontSize(float val);
		void setFontFamily(const std::wstring& val);
		void setColor(Color color);
		void paint();
	public:
		winrt::Windows::UI::Composition::CompositionDrawingSurface surface{ nullptr };
		Microsoft::WRL::ComPtr<IDWriteTextLayout> textLayout;
	private:
		using Node::makeChild;
		using Node::setAlignItems;
		using Node::setJustifyContent;
		using Node::setFlexDirection;
		static YGSize nodeMeasureCB(YGNodeConstRef node, float width, YGMeasureMode widthMode, float height, YGMeasureMode heightMode);
		void layout() override;
	private:
		Color color{ 0x333333FF };
		std::wstring text;
		std::wstring fontFamily;
		float fontSize{ 12.f };
	};
}
