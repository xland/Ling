#pragma once
#include <wrl.h>
#include <winrt/Windows.UI.Composition.h>
#include "Node.h"

namespace Ling {
	class WinBase;
	class NodeText :public Node
	{
		friend class WinBase;
	public:
		NodeText(WinBase* win);
		~NodeText();
		void setText(const std::wstring& text, float fontSize = 13.f, const std::wstring& fontFamily = L"");
		void setColor(Color color);
	public:
		winrt::Windows::UI::Composition::CompositionDrawingSurface surface{ nullptr };
		Microsoft::WRL::ComPtr<IDWriteTextLayout> textLayout;
	private:
		using Node::makeChild;
		using Node::setFlexShrink;
		using Node::setWidth;
		using Node::setHeight;
		using Node::setSize;
		using Node::setWidthPercent;
		using Node::setHeightPercent;
		using Node::setSizePercent;
		using Node::setMargin;
		using Node::setMarginLeft;
		using Node::setMarginTop;
		using Node::setMarginRight;
		using Node::setMarginBottom;
		using Node::getMarginLeft;
		using Node::getMarginTop;
		using Node::getMarginRight;
		using Node::getMarginBottom;
		using Node::setPadding;
		using Node::setPaddingLeft;
		using Node::setPaddingTop;
		using Node::setPaddingRight;
		using Node::setPaddingBottom;
		using Node::getPaddingLeft;
		using Node::getPaddingTop;
		using Node::getPaddingRight;
		using Node::getPaddingBottom;
		using Node::setFlexWrap;
		using Node::setAlignItems;
		using Node::setJustifyContent;
		using Node::setFlexDirection;

		void layout() override;
		void onDpiChanged() override;
		// dpi 变化后按新 dpi 重建 textLayout 并更新 yoga 尺寸
		void rebuildTextLayout();

		Color color{ 0x333333FF };
		// 逻辑参数缓存 —— dpi 变化时用它重建 textLayout
		std::wstring text;
		std::wstring fontFamily;
		float fontSize{ 13.f };
	private:
	};
}
