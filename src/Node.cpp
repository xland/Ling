#include "pch.h"
#include "../include/WinBase.h"
#include "../include/Node.h"

namespace Ling {

	Node::Node(Node* parent) :parent{parent}, win{parent-> win }, node(YGNodeNew()), visual{ win->compositor.CreateSpriteVisual() }
	{
		parent->visual.Children().InsertAtTop(visual);
		YGNodeInsertChild(parent->node, node, YGNodeGetChildCount(node));
		parent->children.push_back(this);
	}
	Node::Node(WinBase* win) :parent{ nullptr }, win{ win }, node(YGNodeNew()), visual{ win->compositor.CreateSpriteVisual() }
	{
	}
	Node::~Node()
	{
		YGNodeFree(node);
	}
	void Node::removeSelf()
	{
		if (!parent) return;
		for (auto& child : children) {
			child->removeSelf();
		}
		YGNodeRemoveChild(parent->node, node);
		parent->visual.Children().Remove(visual);
		auto& siblings = parent->children;
		siblings.erase(std::remove(siblings.begin(), siblings.end(), this), siblings.end());
	}

	void Node::removeChild(Node* ele)
	{
		ele->removeSelf();
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
		YGNodeStyleSetWidth(node, w);
	}
	void Node::setHeight(const float& h)
	{
		YGNodeStyleSetHeight(node, h);
	}

	void Node::setSize(const float& w, const float& h)
	{
		YGNodeStyleSetWidth(node, w);
		YGNodeStyleSetHeight(node, h);
	}

	void Node::setWidthPercent(const float& percent)
	{
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
		YGNodeStyleSetMargin(node, YGEdgeAll, val);
	}

	void Node::setMargin(const float& left, const float& top, const float& right, const float& bottom)
	{
		YGNodeStyleSetMargin(node, YGEdgeLeft, left);
		YGNodeStyleSetMargin(node, YGEdgeTop, top);
		YGNodeStyleSetMargin(node, YGEdgeRight, right);
		YGNodeStyleSetMargin(node, YGEdgeBottom, bottom);
	}

	float Node::getMarginLeft()
	{
		return YGNodeStyleGetMargin(node, YGEdge::YGEdgeLeft).value;
	}

	float Node::getMarginTop()
	{
		return YGNodeStyleGetMargin(node, YGEdge::YGEdgeTop).value;
	}

	float Node::getMarginRight()
	{
		return YGNodeStyleGetMargin(node, YGEdge::YGEdgeRight).value;
	}

	float Node::getMarginBottom()
	{
		return YGNodeStyleGetMargin(node, YGEdge::YGEdgeBottom).value;
	}

	void Node::setMarginLeft(const float& val)
	{
		YGNodeStyleSetMargin(node, YGEdge::YGEdgeLeft, val);
	}

	void Node::setMarginTop(const float& val)
	{
		YGNodeStyleSetMargin(node, YGEdge::YGEdgeTop, val);
	}

	void Node::setMarginRight(const float& val)
	{
		YGNodeStyleSetMargin(node, YGEdge::YGEdgeRight, val);
	}

	void Node::setMarginBottom(const float& val)
	{
		YGNodeStyleSetMargin(node, YGEdge::YGEdgeBottom, val);
	}

	void Node::setPadding(const float& val)
	{
		YGNodeStyleSetPadding(node, YGEdgeAll, val);
	}

	void Node::setPadding(const float& left, const float& top, const float& right, const float& bottom)
	{
		YGNodeStyleSetPadding(node, YGEdgeLeft, left);
		YGNodeStyleSetPadding(node, YGEdgeTop, top);
		YGNodeStyleSetPadding(node, YGEdgeRight, right);
		YGNodeStyleSetPadding(node, YGEdgeBottom, bottom);
	}

	void Node::setPaddingLeft(const float& val)
	{
		YGNodeStyleSetPadding(node, YGEdgeLeft, val);
	}

	void Node::setPaddingTop(const float& val)
	{
		YGNodeStyleSetPadding(node, YGEdgeTop, val);
	}

	void Node::setPaddingRight(const float& val)
	{
		YGNodeStyleSetPadding(node, YGEdgeRight, val);
	}

	void Node::setPaddingBottom(const float& val)
	{
		YGNodeStyleSetPadding(node, YGEdgeBottom, val);
	}

	float Node::getPaddingLeft()
	{
		return YGNodeStyleGetPadding(node, YGEdge::YGEdgeLeft).value;
	}

	float Node::getPaddingTop()
	{
		return YGNodeStyleGetPadding(node, YGEdge::YGEdgeTop).value;
	}

	float Node::getPaddingRight()
	{
		return YGNodeStyleGetPadding(node, YGEdge::YGEdgeRight).value;
	}

	float Node::getPaddingBottom()
	{
		return YGNodeStyleGetPadding(node, YGEdge::YGEdgeBottom).value;
	}
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
