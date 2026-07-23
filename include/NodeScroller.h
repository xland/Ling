#pragma once

#include "Node.h"

namespace Ling {
	class WinBase;
	class NodeScroller:public Node
	{
		friend class WinBase;
	public:
		NodeScroller(WinBase* win);
		~NodeScroller();
		void setContentHeight(float h);
		void onWheel(void* e);
		void onDown(void* e);
		void onUp(void* e);
		void onMove(void* e);
	public:
		//winrt::Windows::UI::Composition::SpriteVisual visualContent{ nullptr };
		std::unique_ptr<Node> content;
	private:
		void setScroll(float y);
		void layout() override;
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