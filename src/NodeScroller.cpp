#include "pch.h"
#include "../include/Event.h"
#include "../include/NodeScroller.h"
#include "../include/WinBase.h"

namespace Ling {

	constexpr float sliderW{ 8.f }, sliderMinH{22.f};

	NodeScroller::NodeScroller(WinBase* win) : Node(win)
	{
		YGNodeStyleSetOverflow(node, YGOverflowScroll);   // 内容溢出走滚动，不参与父级 flex-basis
		YGNodeStyleSetMinHeight(node, 0.f);  // 关键：解除 flex 项目的 min-content 下限
		YGNodeStyleSetFlexShrink(node, 1.f);              // 允许被父级压缩到剩余空间

		colorVisibleScroller = win->compositor.CreateColorBrush(Color(0x88888822).getUIColor());
		colorHoverScroller = win->compositor.CreateColorBrush(Color(0x88888833).getUIColor());
		colorVisibleThumb = win->compositor.CreateColorBrush(Color(0x88888866).getUIColor());
		colorHoverThumb = win->compositor.CreateColorBrush(Color(0x88888888).getUIColor());
		colorTransparent = win->compositor.CreateColorBrush(Color(0x00000000).getUIColor());
		visual.Clip(win->compositor.CreateInsetClip());

		content = makeChild<Node>();

		visualScroller = win->compositor.CreateSpriteVisual();
		visual.Children().InsertAtTop(visualScroller);
		visualScroller.IsVisible(false);

		visualThumb = win->compositor.CreateSpriteVisual();
		visualScroller.Children().InsertAtTop(visualThumb);

		onWheelId = win->on(Event::MouseWheel, [this](void* e) { this->onWheel(e); });
		onMoveId = win->on(Event::MouseMove, [this](void* e) { this->onMove(e);  });
		onUpId = win->on(Event::MouseUp, [this](void* e) { this->onUp(e);    });
		onDownId = win->on(Event::MouseDown, [this](void* e) { this->onDown(e);  });
	}

	NodeScroller::~NodeScroller()
	{
		win->off(Event::MouseWheel, onWheelId);
		win->off(Event::MouseMove, onMoveId);
		win->off(Event::MouseUp, onUpId);
		win->off(Event::MouseDown, onDownId);
	}

	void NodeScroller::setContentHeight(float h)
	{
		content->setWidthPercent(100.f);
		content->setHeight(h);
		scrollY = 0;
	}

	void NodeScroller::onWheel(void* e)
	{
		if (!visualScroller.IsVisible()) return;
		auto tuplePtr = static_cast<std::tuple<POINT, float>*>(e);
		auto [pos, space] = *tuplePtr;
		if (!isPosIn(pos)) return;
		setScroll(scrollY - space);
	}

	void NodeScroller::onDown(void* e)
	{
		if (!visual.IsVisible()) return;
		auto tuplePtr = static_cast<std::tuple<POINT, bool>*>(e);
		auto [pos, isRight] = *tuplePtr;
		if (isRight) return;
		auto sbW{ sliderW * win->dpi };
		// 只在点击滚动条条形区域内才启动拖动
		if (pos.y >= y && pos.y <= y + h && pos.x >= x + w - sbW && pos.x <= x + w) {
			SetCapture(win->hwnd);
			scrollerDragging = true;
			dragStartMouseY = (float)pos.y;
			dragStartScrollY = scrollY;
		}
	}

	void NodeScroller::onUp(void* e)
	{
		if (scrollerDragging) {
			ReleaseCapture();
			scrollerDragging = false;
		}
	}

	void NodeScroller::onMove(void* e)
	{
		if (!visual.IsVisible()) return;
		auto tuplePtr = static_cast<std::tuple<POINT>*>(e);
		auto [pos] = *tuplePtr;
		if (!scrollerDragging && !isPosIn(pos)) {
			visualScroller.Brush(colorTransparent);
			visualThumb.Brush(colorTransparent);
			return;
		}
		auto sbW{ sliderW * win->dpi };
		if (scrollerDragging) {
			float maxScroll = std::max(0.f, content->h - h);
			float minH = sliderMinH * win->dpi;
			float thumbH = std::max(minH, h * h / content->h);
			float trackFree = h - thumbH;
			if (trackFree <= 0) return;
			float ratio = (pos.y - dragStartMouseY) / trackFree;
			setScroll(dragStartScrollY + ratio * maxScroll);
		}
		else {
			if (pos.x < x + w - sbW) {
				visualScroller.Brush(colorVisibleScroller);
				visualThumb.Brush(colorVisibleThumb);
			}
			else {
				visualScroller.Brush(colorHoverScroller);
				visualThumb.Brush(colorHoverThumb);
			}
		}

	}

	void NodeScroller::setScroll(float y)
	{
		float maxScroll = std::max(0.f, content->h - h);
		y = std::clamp(y, 0.f, maxScroll);
		scrollY = y;
		content->visual.Offset({ 0.f, -scrollY, 0.f }); //todo 重要，命中测试时，要考虑此偏移
		if (content->h > h) {
			float minH = sliderMinH * win->dpi;
			float thumbH = std::max(minH, h * h / content->h);
			float maxScroll = content->h - h;
			float top = maxScroll > 0 ? scrollY * (h - thumbH) / maxScroll : 0.f;
			visualThumb.Offset({ 0.f, top ,0.f });
			visualThumb.Size({ 6 * win->dpi, thumbH });
		}
	}

	void NodeScroller::layout()
	{
		Node::layout();
		if (content->h > h) { //有滚动条
			auto sbW{ 6 * win->dpi };
			visualScroller.Offset({ w - sbW, 0.f ,0.f });
			visualScroller.Size({ sbW,h });
			visualScroller.IsVisible(true);
			setScroll(scrollY);
		}
		else {
			visualScroller.IsVisible(false);
		}
	}

}