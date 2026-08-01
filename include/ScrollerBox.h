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
		// 当前滚动偏移（物理像素，>=0）。内容 visual 被向上平移了这么多，
		// 所以窗口坐标 -> 内容坐标需要 +getScrollY()。
		float getScrollY() const { return scrollY; }
		// 可滚动的最大距离（物理像素）。content 不溢出时为 0。
		float getMaxScrollY() const;
		// 滚动到指定偏移（物理像素，内部 clamp 到 [0, maxScroll] 并 snap 到整像素）。
		void scrollTo(float y);
		// 相对滚动。delta > 0 向下。
		void scrollBy(float delta);
		// 滚动使内容坐标系下的纵向区间 [top, bottom] 尽量进入可视范围。
		void scrollIntoView(float top, float bottom);
		// 滚动条条形区域宽度（物理像素）；没有滚动条时为 0。
		// 命中测试要排除这条竖带，否则会和拖动滑块冲突。
		float getScrollBarWidth() const;
		// pos 是否落在可视内容区内（排除右侧滚动条竖带）。
		bool isPosInContent(POINT pos) const;
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
		std::shared_ptr<bool> alive{ std::make_shared<bool>(true) };
	};
}
