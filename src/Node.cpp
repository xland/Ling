#include "pch.h"
#include "../include/WinBase.h"
#include "../include/Node.h"

namespace Ling {

	Node::Node(WinBase* win) :win{ win }, node(YGNodeNew()), visual{ win->compositor.CreateSpriteVisual() }
	{
	}

	Node::~Node()
	{
		YGNodeFree(node);
	}

	std::unique_ptr<Node> Node::detachChild(Node* child) {
		auto it = std::find_if(children.begin(), children.end(), [child](const std::unique_ptr<Node>& p) { return p.get() == child; });
		if (it == children.end()) return nullptr;
		YGNodeRemoveChild(node, child->node);
		visual.Children().Remove(child->visual);
		child->parent = nullptr;
		std::unique_ptr<Node> owned = std::move(*it);
		children.erase(it);
		return owned;
	}

	void Node::removeChild(Node* child) {
		detachChild(child);
	}

	bool Node::isPosIn(POINT pos)
	{
		if (pos.x > x && pos.x<x + w && pos.y>y && pos.y < y + h) {
			return true;
		}
		return false;
	}
	void Node::hide()
	{
		visual.IsVisible(false);
	}

	void Node::show()
	{
		visual.IsVisible(true);
	}

	void Node::setBg(const Color& color)
	{
		visual.Brush(win->compositor.CreateColorBrush(color.getUIColor()));
	}

	void Node::layout()
	{
		x = YGNodeLayoutGetLeft(node);
		y = YGNodeLayoutGetTop(node);
		w = YGNodeLayoutGetWidth(node);
		h = YGNodeLayoutGetHeight(node);
		visual.Offset({ x, y, 0.0f });
		visual.Size({ w, h });
		if (parent) {
			x = parent->x + x;
			y = parent->y + y;
		}
		for (auto& child : children) {
			child->layout();
		}
	}

	void Node::applyDpiChange()
	{
		const float d = win->dpi;
		if (logical.width)  YGNodeStyleSetWidth (node, *logical.width  * d);
		if (logical.height) YGNodeStyleSetHeight(node, *logical.height * d);
		if (logical.margin[0])  YGNodeStyleSetMargin(node, YGEdgeLeft,   *logical.margin[0]  * d);
		if (logical.margin[1])  YGNodeStyleSetMargin(node, YGEdgeTop,    *logical.margin[1]  * d);
		if (logical.margin[2])  YGNodeStyleSetMargin(node, YGEdgeRight,  *logical.margin[2]  * d);
		if (logical.margin[3])  YGNodeStyleSetMargin(node, YGEdgeBottom, *logical.margin[3]  * d);
		if (logical.padding[0]) YGNodeStyleSetPadding(node, YGEdgeLeft,   *logical.padding[0] * d);
		if (logical.padding[1]) YGNodeStyleSetPadding(node, YGEdgeTop,    *logical.padding[1] * d);
		if (logical.padding[2]) YGNodeStyleSetPadding(node, YGEdgeRight,  *logical.padding[2] * d);
		if (logical.padding[3]) YGNodeStyleSetPadding(node, YGEdgeBottom, *logical.padding[3] * d);

		onDpiChanged();

		for (auto& child : children) {
			child->applyDpiChange();
		}
	}

	void Node::setFlexGrow(const float& val)
	{
		YGNodeStyleSetFlexGrow(node, val);
	}

	void Node::setFlexShrink(const float& val)
	{
		YGNodeStyleSetFlexShrink(node, val);
	}

	void Node::setWidth(const float& w)
	{
		logical.width = w;
		YGNodeStyleSetWidth(node, w * win->dpi);
	}
	void Node::setHeight(const float& h)
	{
		logical.height = h;
		YGNodeStyleSetHeight(node, h * win->dpi);
	}

	void Node::setSize(const float& w, const float& h)
	{
		setWidth(w);
		setHeight(h);
	}

	void Node::setWidthPercent(const float& percent)
	{
		// 百分比不随 dpi 变化，直接透传给 yoga
		YGNodeStyleSetWidthPercent(node, percent);
	}

	void Node::setHeightPercent(const float& percent)
	{
		YGNodeStyleSetHeightPercent(node, percent);
	}

	void Node::setSizePercent(const float& w, const float& h)
	{
		YGNodeStyleSetWidthPercent(node, w);
		YGNodeStyleSetHeightPercent(node, h);
	}

	void Node::setMargin(const float& val)
	{
		logical.margin[0] = val;
		logical.margin[1] = val;
		logical.margin[2] = val;
		logical.margin[3] = val;
		YGNodeStyleSetMargin(node, YGEdgeAll, val * win->dpi);
	}

	void Node::setMargin(const float& left, const float& top, const float& right, const float& bottom)
	{
		setMarginLeft(left);
		setMarginTop(top);
		setMarginRight(right);
		setMarginBottom(bottom);
	}

	float Node::getMarginLeft()   { return logical.margin[0].value_or(0.f); }
	float Node::getMarginTop()    { return logical.margin[1].value_or(0.f); }
	float Node::getMarginRight()  { return logical.margin[2].value_or(0.f); }
	float Node::getMarginBottom() { return logical.margin[3].value_or(0.f); }

	void Node::setMarginLeft(const float& val)
	{
		logical.margin[0] = val;
		YGNodeStyleSetMargin(node, YGEdgeLeft, val * win->dpi);
	}

	void Node::setMarginTop(const float& val)
	{
		logical.margin[1] = val;
		YGNodeStyleSetMargin(node, YGEdgeTop, val * win->dpi);
	}

	void Node::setMarginRight(const float& val)
	{
		logical.margin[2] = val;
		YGNodeStyleSetMargin(node, YGEdgeRight, val * win->dpi);
	}

	void Node::setMarginBottom(const float& val)
	{
		logical.margin[3] = val;
		YGNodeStyleSetMargin(node, YGEdgeBottom, val * win->dpi);
	}

	void Node::setPadding(const float& val)
	{
		logical.padding[0] = val;
		logical.padding[1] = val;
		logical.padding[2] = val;
		logical.padding[3] = val;
		YGNodeStyleSetPadding(node, YGEdgeAll, val * win->dpi);
	}

	void Node::setPadding(const float& left, const float& top, const float& right, const float& bottom)
	{
		setPaddingLeft(left);
		setPaddingTop(top);
		setPaddingRight(right);
		setPaddingBottom(bottom);
	}

	void Node::setPaddingLeft(const float& val)
	{
		logical.padding[0] = val;
		YGNodeStyleSetPadding(node, YGEdgeLeft, val * win->dpi);
	}

	void Node::setPaddingTop(const float& val)
	{
		logical.padding[1] = val;
		YGNodeStyleSetPadding(node, YGEdgeTop, val * win->dpi);
	}

	void Node::setPaddingRight(const float& val)
	{
		logical.padding[2] = val;
		YGNodeStyleSetPadding(node, YGEdgeRight, val * win->dpi);
	}

	void Node::setPaddingBottom(const float& val)
	{
		logical.padding[3] = val;
		YGNodeStyleSetPadding(node, YGEdgeBottom, val * win->dpi);
	}

	float Node::getPaddingLeft()   { return logical.padding[0].value_or(0.f); }
	float Node::getPaddingTop()    { return logical.padding[1].value_or(0.f); }
	float Node::getPaddingRight()  { return logical.padding[2].value_or(0.f); }
	float Node::getPaddingBottom() { return logical.padding[3].value_or(0.f); }

	void Node::setAlignItems(const Align& val)
	{
		YGNodeStyleSetAlignItems(node, (YGAlign)val);
	}

	void Node::setFlexWrap(const Wrap& val)
	{
		YGNodeStyleSetFlexWrap(node, (YGWrap)val);
	}
	void Node::setJustifyContent(const Justify& val)
	{
		YGNodeStyleSetJustifyContent(node, (YGJustify)val);
	}
	void Node::setFlexDirection(const FlexDirection& flexDirection)
	{
		YGNodeStyleSetFlexDirection(node, (YGFlexDirection)flexDirection);
	}
}
