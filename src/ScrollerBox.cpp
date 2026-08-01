#include "pch.h"
#include "../include/ScrollerBox.h"
#include "../include/WinBase.h"

namespace Ling {

	// 逻辑像素常量
	constexpr float sliderW{ 8.f }, sliderMinH{ 22.f };

	ScrollerBox::ScrollerBox(WinBase* win) : Node(win)
	{
		YGNodeStyleSetOverflow(node, YGOverflowScroll);   // 内容溢出走滚动，不参与父级 flex-basis
		YGNodeStyleSetMinHeight(node, 0.f);               // 关键：解除 flex 项目的 min-content 下限
		YGNodeStyleSetFlexShrink(node, 1.f);              // 允许被父级压缩到剩余空间

		colorVisibleScroller = win->compositor.CreateColorBrush(Color(0x88888822).getUIColor());
		colorHoverScroller = win->compositor.CreateColorBrush(Color(0x88888833).getUIColor());
		colorVisibleThumb = win->compositor.CreateColorBrush(Color(0x88888866).getUIColor());
		colorHoverThumb = win->compositor.CreateColorBrush(Color(0x88888888).getUIColor());
		colorTransparent = win->compositor.CreateColorBrush(Color(0x00000000).getUIColor());
		visual.Clip(win->compositor.CreateInsetClip());

		content = new Node(win);
		content->parent = this;
		visual.Children().InsertAtTop(content->visual);
		YGNodeInsertChild(this->node, content->node, YGNodeGetChildCount(this->node));
		auto safePtr = std::unique_ptr<Node>(content);
		children.push_back(std::move(safePtr));

		visualScroller = win->compositor.CreateSpriteVisual();
		visual.Children().InsertAtTop(visualScroller);
		visualScroller.IsVisible(false);

		visualThumb = win->compositor.CreateSpriteVisual();
		visualScroller.Children().InsertAtTop(visualThumb);


		auto weakThis = getWeakThis();
		wheelTok = win->onMouseWheel.add([this, weakThis](POINT pos, float space) { if (!weakThis.lock()) return; onWheel(pos, space); });
		moveTok  = win->onMouseMove .add([this, weakThis](POINT pos)               { if (!weakThis.lock()) return; onMove(pos); });
		upTok    = win->onMouseUp   .add([this, weakThis](POINT pos, bool isRight) { if (!weakThis.lock()) return; onUp(pos, isRight); });
		downTok  = win->onMouseDown .add([this, weakThis](POINT pos, bool isRight) { if (!weakThis.lock()) return; onDown(pos, isRight); });
	}

	ScrollerBox::~ScrollerBox()
	{
		win->onMouseWheel.remove(wheelTok);
		win->onMouseMove .remove(moveTok);
		win->onMouseUp   .remove(upTok);
		win->onMouseDown .remove(downTok);
	}

	void ScrollerBox::onWheel(POINT pos, float space)
	{
		if (!visualScroller.IsVisible()) return;
		if (!isPosIn(pos)) return;
		setScroll(scrollY - space);
	}

	void ScrollerBox::onDown(POINT pos, bool isRight)
	{
		if (!visual.IsVisible()) return;
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

	void ScrollerBox::onUp(POINT pos, bool isRight)
	{
		if (scrollerDragging) {
			ReleaseCapture();
			scrollerDragging = false;
		}
	}

	void ScrollerBox::onMove(POINT pos)
	{
		if (!visual.IsVisible()) return;
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

	void ScrollerBox::setScroll(float y)
	{
		float maxScroll = std::max(0.f, content->h - h);
		y = std::clamp(y, 0.f, maxScroll);
		// 关键：偏移 snap 到整像素。scrollY 带小数会让 content 及其所有子节点
		// 落到分数像素位置，ClearType 文本在滚动过程中会周期性发糊。
		// 命中测试用的仍是这个整数 scrollY —— 保持"视觉/逻辑"一致。
		scrollY = std::round(y);
		content->visual.Offset({ 0.f, -scrollY, 0.f });
		// 命中测试请用 getScrollY()：窗口坐标 -> 内容坐标要 +scrollY。
		if (content->h > h) {
			float minH = sliderMinH * win->dpi;
			float thumbH = std::max(minH, h * h / content->h);
			float top = maxScroll > 0 ? scrollY * (h - thumbH) / maxScroll : 0.f;
			// thumb 也 snap 一下，避免拖动时滑块自身发糊
			visualThumb.Offset({ 0.f, std::round(top), 0.f });
			visualThumb.Size({ sliderW * win->dpi, std::round(thumbH) });
		}
	}

	float ScrollerBox::getMaxScrollY() const
	{
		return std::max(0.f, content->h - h);
	}

	void ScrollerBox::scrollTo(float y)
	{
		if (scrollY == std::round(std::clamp(y, 0.f, getMaxScrollY()))) return;
		setScroll(y);
	}

	void ScrollerBox::scrollBy(float delta)
	{
		scrollTo(scrollY + delta);
	}

	void ScrollerBox::scrollIntoView(float top, float bottom)
	{
		// top/bottom 是内容坐标（未减 scrollY）。可视窗口是 [scrollY, scrollY + h]。
		if (bottom <= top) return;
		if (top < scrollY) scrollTo(top);
		else if (bottom > scrollY + h) {
			// 区间比视口还高时优先对齐顶部，否则底部贴边
			scrollTo(bottom - top > h ? top : bottom - h);
		}
	}

	float ScrollerBox::getScrollBarWidth() const
	{
		return visualScroller && visualScroller.IsVisible() ? sliderW * win->dpi : 0.f;
	}

	bool ScrollerBox::isPosInContent(POINT pos) const
	{
		return pos.x >= x && pos.x < x + w - getScrollBarWidth()
			&& pos.y >= y && pos.y < y + h;
	}

	void ScrollerBox::onDpiChanged()
	{
		// 滑块所有尺寸都是 dpi 派生量。这里不用做任何事：
		// applyDpiChange 结束后 WinBase 会 relayout，layout() 里会调 setScroll
		// 重算 visualThumb 的 offset/size；visualScroller 的 offset/size 也在 layout 里更新。
	}

	void ScrollerBox::setChild(Node* child)
	{
		child->parent = content;
		content->visual.Children().InsertAtTop(child->visual);
		YGNodeInsertChild(content->node, child->node, YGNodeGetChildCount(content->node));
	}

	void ScrollerBox::layout()
	{
		Node::layout();
		if (content->h > h) { //有滚动条
			auto sbW{ sliderW * win->dpi };
			visualScroller.Offset({ w - sbW, 0.f, 0.f });
			visualScroller.Size({ sbW, h });
			visualScroller.IsVisible(true);
			setScroll(scrollY);
		}
		else {
			// 内容缩到不需要滚动了（窗口变大 / 内容变少）。Node::layout 刚把
			// content 的 visual offset 重置成 0，这里必须把 scrollY 也跟着归零，
			// 否则它会残留旧值，让依赖 getScrollY() 的命中测试整体偏移。
			scrollY = 0.f;
			visualScroller.IsVisible(false);
		}
	}

}
