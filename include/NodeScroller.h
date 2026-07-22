#pragma once

#include "Node.h"

namespace Ling {
	class WinBase;
	class NodeScroller:public Node
	{
		friend class WinBase;
	public:
		~NodeScroller();
		void setContentHeight(float h);
		void onWheel(void* e);
		void onDown(void* e);
		void onUp(void* e);
		void onMove(void* e);
	private:
		NodeScroller(WinBase* win);
		void setScroll(float y);
	private:
		winrt::Windows::UI::Composition::SpriteVisual visualContent{ nullptr };
		winrt::Windows::UI::Composition::SpriteVisual visualScroller{ nullptr };
		winrt::Windows::UI::Composition::SpriteVisual visualThumb{ nullptr };
		winrt::Windows::UI::Composition::CompositionColorBrush colorVisibleScroller{nullptr}, colorHoverScroller{ nullptr }, colorVisibleThumb{ nullptr }, colorHoverThumb{ nullptr };
		float scrollY, dragStartMouseY, dragStartScrollY;
		bool scrollerDragging;
	};
}