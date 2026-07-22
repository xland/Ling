#include "pch.h"
#include "../include/EventType.h"
#include "../include/NodeScroller.h"
#include "../include/WinBase.h"

namespace Ling {

	NodeScroller::NodeScroller(WinBase* win) :Node(win) 
	{
		colorVisibleScroller = win->compositor.CreateColorBrush(Color(0xEEEEEE33).getUIColor());
		colorHoverScroller = win->compositor.CreateColorBrush(Color(0xEEEEEE88).getUIColor());
		colorVisibleThumb = win->compositor.CreateColorBrush(Color(0xEEEEEE88).getUIColor());
		colorHoverThumb = win->compositor.CreateColorBrush(Color(0xEEEEEEFF).getUIColor());


		visual.Clip(win->compositor.CreateInsetClip());
		visualContent = win->compositor.CreateSpriteVisual();
		visualContent.Offset({ 0.f,0.f,0.f });
		visualContent.RelativeSizeAdjustment({ 1.f,1.f });
		visual.Children().InsertAtTop(visualContent);

		visualScroller = win->compositor.CreateSpriteVisual();
		visual.Children().InsertAtTop(visualScroller);
		visualScroller.IsVisible(false);

		visualThumb = win->compositor.CreateSpriteVisual();
		visualScroller.Children().InsertAtTop(visualThumb);


		win->on(EventType::MouseWheel, [this](void* e) {this->onWheel(e);});
		win->on(EventType::MouseMove, [this](void* e) {this->onMove(e);});
		win->on(EventType::MouseUp, [this](void* e) {this->onUp(e);});
		win->on(EventType::MouseDown, [this](void* e) {this->onDown(e);});


		//auto expression = win->compositor.CreateExpressionAnimation(L"Parent.Size.X - 200.0f");
		//expression.SetReferenceParameter(L"Parent", visual);
		//visualContent.StartAnimation(L"Offset.X", expression);
	}

	NodeScroller::~NodeScroller()
	{
	}

	void NodeScroller::setContentHeight(float h)
	{

		//auto contentSize = visualContent.Size();
		//setContentHeight(contentSize.y);

		auto parentSize = visual.Size();
		visualContent.Size({ parentSize.x,h });
		scrollY = 0;
		if (h > parentSize.y) { //有滚动条
			auto sbW{ 6 * win->dpi };
			visualScroller.Offset({ parentSize.x - sbW, 0.f ,0.f});
			visualScroller.Size({ sbW,parentSize.y });
			visualScroller.IsVisible(true);
			setScroll(0.f);
		}
		else {
			visualScroller.IsVisible(false);
		}
	}

	void NodeScroller::onWheel(void* e)
	{
		if (!visualScroller.IsVisible()) return;
		auto tuplePtr = static_cast<std::tuple<float,float, float>*>(e);
		auto [x,y, space] = *tuplePtr;
		if (!isPosIn(x,y)) return;
		setScroll(scrollY - space);
	}

	void NodeScroller::onDown(void* e)
	{
		if (!visual.IsVisible()) return;
		auto tuplePtr = static_cast<std::tuple<float, float, bool>*>(e);
		auto [x, y, isRight] = *tuplePtr;
		if (isRight) return;
		auto pos = visual.Offset();
		auto size = visual.Size();
		auto sbW{ 6 * win->dpi };
		if (y < pos.y || y > pos.y + size.y || x < pos.x + size.x - sbW || x > pos.x + size.x) {
			SetCapture(win->hwnd);
			scrollerDragging = true;
			dragStartMouseY = (float)y;
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
		auto tuplePtr = static_cast<std::tuple<float, float>*>(e);
		auto [x, y] = *tuplePtr;
		auto parentPos = visual.Offset();
		auto parentSize = visual.Size();
		if (y < parentPos.y || y > parentPos.y + parentSize.y || x < parentPos.x || x>parentPos.x + parentSize.x) {
			visualScroller.Brush(nullptr);
			visualThumb.Brush(nullptr);
			return;
		}
		auto sbW{ 6 * win->dpi };
		if (scrollerDragging) {
			auto contentSize = visualContent.Size();
			float maxScroll = std::max(0.f, contentSize.y - parentSize.y);
			float minH = 22.f * win->dpi;
			float thumbH = std::max(minH, parentSize.y * parentSize.y / contentSize.y);
			float trackFree = parentSize.y - thumbH;
			if (trackFree <= 0) return;
			float ratio = (y - dragStartMouseY) / trackFree;
			setScroll(dragStartScrollY + ratio * maxScroll);
		}
		else {
			if (x < parentSize.x + parentSize.x - sbW) {
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
		auto parentSize = visual.Size();
		float maxScroll = std::max(0.f, contentSize.y - parentSize.y);
		y = std::clamp(y, 0.f, maxScroll); //将一个值限制在指定的上下界范围内
		scrollY = y;
		visualContent.Offset({ 0.f, -scrollY, 0.f });
		if (contentSize.y > parentSize.y) {
			float minH = 22.f * win->dpi;
			float thumbH = std::max(minH, parentSize.y * parentSize.y / contentSize.y);
			float maxScroll = contentSize.y - parentSize.y;
			float top = maxScroll > 0 ? scrollY * (parentSize.y - thumbH) / maxScroll : 0.f;
			visualThumb.Offset({ 0.f, scrollY ,0.f });
		}
	}

}