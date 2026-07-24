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
		void onWheel(void* e);
		void onMove(void* e);
		void onDown(void* e);
		void onUp(void* e);
	public:
		Node* content;
	private:
		void setScroll(float y);
		void layout() override;
		void onDpiChanged() override;
		void setChild(Node* child) override;
	private:
		size_t onWheelId, onMoveId, onUpId, onDownId;
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