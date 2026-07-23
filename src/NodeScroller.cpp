#include "pch.h"
#include "../include/Event.h"
#include "../include/NodeScroller.h"
#include "../include/WinBase.h"

namespace Ling {

	constexpr float sliderW{ 6.f }, sliderMinH{22.f};

	NodeScroller::NodeScroller(Node* parent) : Node(parent)
	{
		colorVisibleScroller = win->compositor.CreateColorBrush(Color(0x88888822).getUIColor());
		colorHoverScroller = win->compositor.CreateColorBrush(Color(0x88888833).getUIColor());
		colorVisibleThumb = win->compositor.CreateColorBrush(Color(0x88888866).getUIColor());
		colorHoverThumb = win->compositor.CreateColorBrush(Color(0x88888888).getUIColor());
		colorTransparent = win->compositor.CreateColorBrush(Color(0x00000000).getUIColor());
		visual.Clip(win->compositor.CreateInsetClip());

		content = std::make_unique<Node>(this);

		visualContent = win->compositor.CreateSpriteVisual();
		visualContent.Offset({ 0.f,0.f,0.f });
		visual.Children().InsertAtTop(visualContent);

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
		visualContent.Size({ w,h });
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
			auto contentSize = visualContent.Size();
			float maxScroll = std::max(0.f, contentSize.y - h);
			float minH = sliderMinH * win->dpi;
			float thumbH = std::max(minH, h * h / contentSize.y);
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
		auto contentSize = visualContent.Size();
		float maxScroll = std::max(0.f, contentSize.y - h);
		y = std::clamp(y, 0.f, maxScroll);
		scrollY = y;
		visualContent.Offset({ 0.f, -scrollY, 0.f });
		if (contentSize.y > h) {
			float minH = sliderMinH * win->dpi;
			float thumbH = std::max(minH, h * h / contentSize.y);
			float maxScroll = contentSize.y - h;
			float top = maxScroll > 0 ? scrollY * (h - thumbH) / maxScroll : 0.f;
			visualThumb.Offset({ 0.f, top ,0.f });
			visualThumb.Size({ 6 * win->dpi, thumbH });
		}
	}

	void NodeScroller::layout()
	{
		Node::layout();
		auto contentSize = visualContent.Size();
		visualContent.Size({ w,contentSize.y });
		if (contentSize.y > h) { //有滚动条
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