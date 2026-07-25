#pragma once

#include "Node.h"

namespace Ling {
	class WinBase;
	class ScrollerBox:public Node
	{
		friend class WinBase;
	public:
		ScrollerBox(WinBase* win);
		~ScrollerBox();
		void onWheel(POINT pos, float space);
		void onMove(POINT pos);
		void onDown(POINT pos, bool isRight);
		void onUp(POINT pos, bool isRight);
	public:
		Node* content;
	private:
		void setScroll(float y);
		void layout() override;
		void onDpiChanged() override;
		void setChild(Node* child) override;
	private:
		winrt::event_token wheelTok{}, moveTok{}, upTok{}, downTok{};
		winrt::Windows::UI::Composition::SpriteVisual visualScroller{ nullptr };
		winrt::Windows::UI::Composition::SpriteVisual visualThumb{ nullptr };
		winrt::Windows::UI::Composition::CompositionColorBrush colorVisibleScroller{ nullptr },
			colorHoverScroller{ nullptr },
			colorVisibleThumb{ nullptr },
			colorHoverThumb{ nullptr },
			colorTransparent{nullptr};
		float scrollY, dragStartMouseY, dragStartScrollY;
		bool scrollerDragging;
	};
}
