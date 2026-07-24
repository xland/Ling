#pragma once
#include "Ling.h"

namespace Ling {
	class WinBase;
	class Text;   // 内部类型的前向声明
	class Button :public Node, public EventBase
	{
	public:
		Button(WinBase* win);
		~Button();
		void setText(const std::wstring& text);
		void setFontSize(float val);
		void setFontFamily(const std::wstring& val);
		void setColor(Color color);
		void setBg(const Color& color) override;
		void setHoverColor(Color color);
		void setHoverBg(Color color);
	private:
		void onMove(void* e);
		void onDown(void* e);
	private:
		Text* text{ nullptr };
		size_t onMoveId, onDownId;
		Color hoverColor{ 0x333333FF }, hoverBg{ 0 }, color{ 0x333333FF };
		// 两个背景刷缓存下来，hover 切换时只做引用替换，不再每次 new。
		winrt::Windows::UI::Composition::CompositionColorBrush normalBrush{ nullptr };
		winrt::Windows::UI::Composition::CompositionColorBrush hoverBrush{ nullptr };
		bool isHover{ false };
	};
}




