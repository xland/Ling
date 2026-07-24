#include "pch.h"
#include "../include/WinBase.h"
#include "../include/Node.h"

namespace Ling {

	Node::Node(WinBase* win) :win{ win }, node(YGNodeNewWithConfig(win->yogaConfig)), visual{ win->compositor.CreateSpriteVisual() }
	{
	}

	Node::~Node()
	{
		YGNodeFree(node);
	}

	void Node::setId(const std::string& id)
	{
		this->id = id;
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
	void Node::setChild(Node* child) 
	{
		child->parent = this;
		visual.Children().InsertAtTop(child->visual);
		YGNodeInsertChild(this->node, child->node, YGNodeGetChildCount(this->node));
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
		bgColor = color;
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

	void Node::_beforeLayout()
	{
		for (auto& child : children) {
			child->beforeLayout();
			child->_beforeLayout();
		}
	}

	void Node::applyDpiChange()
	{
		const float d = win->dpi;
		if (width)  YGNodeStyleSetWidth (node, *width  * d);
		if (height) YGNodeStyleSetHeight(node, *height * d);
		if (margin[0])  YGNodeStyleSetMargin(node, YGEdgeLeft,   *margin[0]  * d);
		if (margin[1])  YGNodeStyleSetMargin(node, YGEdgeTop,    *margin[1]  * d);
		if (margin[2])  YGNodeStyleSetMargin(node, YGEdgeRight,  *margin[2]  * d);
		if (margin[3])  YGNodeStyleSetMargin(node, YGEdgeBottom, *margin[3]  * d);
		if (padding[0]) YGNodeStyleSetPadding(node, YGEdgeLeft,   *padding[0] * d);
		if (padding[1]) YGNodeStyleSetPadding(node, YGEdgeTop,    *padding[1] * d);
		if (padding[2]) YGNodeStyleSetPadding(node, YGEdgeRight,  *padding[2] * d);
		if (padding[3]) YGNodeStyleSetPadding(node, YGEdgeBottom, *padding[3] * d);
		onDpiChanged();
		for (auto& child : children) {
			child->applyDpiChange();
		}
	}

	void Node::setFlexGrow(float val)
	{
		YGNodeStyleSetFlexGrow(node, val);
	}

	void Node::setFlexShrink(float val)
	{
		YGNodeStyleSetFlexShrink(node, val);
	}

	void Node::setWidth(float w)
	{
		width = w;
		YGNodeStyleSetWidth(node, w * win->dpi);
	}
	void Node::setHeight(float h)
	{
		height = h;
		YGNodeStyleSetHeight(node, h * win->dpi);
	}

	void Node::setSize(float w, float h)
	{
		setWidth(w);
		setHeight(h);
	}

	void Node::setWidthPercent(float percent)
	{
		// 百分比不随 dpi 变化，直接透传给 yoga
		YGNodeStyleSetWidthPercent(node, percent);
	}

	void Node::setHeightPercent(float percent)
	{
		YGNodeStyleSetHeightPercent(node, percent);
	}

	void Node::setSizePercent(float w, float h)
	{
		YGNodeStyleSetWidthPercent(node, w);
		YGNodeStyleSetHeightPercent(node, h);
	}

	void Node::setMargin(float val)
	{
		margin[0] = val;
		margin[1] = val;
		margin[2] = val;
		margin[3] = val;
		YGNodeStyleSetMargin(node, YGEdgeAll, val * win->dpi);
	}

	void Node::setMargin(float left, float top, float right, float bottom)
	{
		setMarginLeft(left);
		setMarginTop(top);
		setMarginRight(right);
		setMarginBottom(bottom);
	}

	float Node::getMarginLeft()   { return margin[0].value_or(0.f); }
	float Node::getMarginTop()    { return margin[1].value_or(0.f); }
	float Node::getMarginRight()  { return margin[2].value_or(0.f); }
	float Node::getMarginBottom() { return margin[3].value_or(0.f); }

	void Node::setMarginLeft(float val)
	{
		margin[0] = val;
		YGNodeStyleSetMargin(node, YGEdgeLeft, val * win->dpi);
	}

	void Node::setMarginTop(float val)
	{
		margin[1] = val;
		YGNodeStyleSetMargin(node, YGEdgeTop, val * win->dpi);
	}

	void Node::setMarginRight(float val)
	{
		margin[2] = val;
		YGNodeStyleSetMargin(node, YGEdgeRight, val * win->dpi);
	}

	void Node::setMarginBottom(float val)
	{
		margin[3] = val;
		YGNodeStyleSetMargin(node, YGEdgeBottom, val * win->dpi);
	}

	void Node::setPadding(float val)
	{
		padding[0] = val;
		padding[1] = val;
		padding[2] = val;
		padding[3] = val;
		YGNodeStyleSetPadding(node, YGEdgeAll, val * win->dpi);
	}

	void Node::setPadding(float left, float top, float right, float bottom)
	{
		setPaddingLeft(left);
		setPaddingTop(top);
		setPaddingRight(right);
		setPaddingBottom(bottom);
	}

	void Node::setPaddingLeft(float val)
	{
		padding[0] = val;
		YGNodeStyleSetPadding(node, YGEdgeLeft, val * win->dpi);
	}

	void Node::setPaddingTop(float val)
	{
		padding[1] = val;
		YGNodeStyleSetPadding(node, YGEdgeTop, val * win->dpi);
	}

	void Node::setPaddingRight(float val)
	{
		padding[2] = val;
		YGNodeStyleSetPadding(node, YGEdgeRight, val * win->dpi);
	}

	void Node::setPaddingBottom(float val)
	{
		padding[3] = val;
		YGNodeStyleSetPadding(node, YGEdgeBottom, val * win->dpi);
	}

	float Node::getPaddingLeft()   { return padding[0].value_or(0.f); }
	float Node::getPaddingTop()    { return padding[1].value_or(0.f); }
	float Node::getPaddingRight()  { return padding[2].value_or(0.f); }
	float Node::getPaddingBottom() { return padding[3].value_or(0.f); }

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
