#include "pch.h"
#include "Element.h"
#include "WindowBase.h"
namespace Ling {

	Element::Element(WindowBase* win) : win(win), node(YGNodeNew()), visual{ win->compositor.CreateSpriteVisual() }
	{
		YGNodeSetContext(node, this);
	}
	Element::~Element()
	{
		YGNodeFree(node);
	}
	bool Element::isAncestor(const Element* target)
	{
		const Element* current = target;
		while (current) {
			if (current == this) return true;
			current = current->parent;
		}
		return false;
	}

	Element* Element::findAncestor(Element* other)
	{
		std::unordered_set<Element*> ancestors;
		Element* current = this;
		while (current) {
			ancestors.insert(current);
			current = current->parent;
		}
		current = other;
		while (current) {
			if (ancestors.count(current)) return current;
			current = current->parent;
		}
		return nullptr;
	}

	void Element::layout()
	{
		x = YGNodeLayoutGetLeft(node);
		y = YGNodeLayoutGetTop(node);
		w = YGNodeLayoutGetWidth(node);
		h = YGNodeLayoutGetHeight(node);
		if (parent) {
			xAbs = parent->xAbs + x;
			yAbs = parent->yAbs + y;
		}
		visual.Offset({ x, y, 0.0f });
		visual.Size({ w, h });
		for (auto& child : children) {
			child->layout();
		}
	}

	bool Element::setCursor()
	{
		if (this->cursor) {
			SetCursor(this->cursor);
			return true;
		}
		else if (parent && parent->setCursor()) {
			return true;
		}
		return false;
	}

	Element* Element::hitTest(const int& x, const int& y)
	{
		if (x < xAbs || x > xAbs + w || y < yAbs || y > yAbs + h) {
			return nullptr;
		}

		for (auto& child : children) {
			auto hit = child->hitTest(x, y);
			if (hit) return hit;
		}
		return this;
	}

	bool Element::containPosition(const int& x, const int& y)
	{
		if (x < xAbs || x > xAbs + w || y < yAbs || y > yAbs + h) {
			return false;
		}
		return true;
	}

	void Element::removeSelf()
	{
		if (!parent) return;
		for (auto& child : children) {
			child->removeSelf();
		}
		YGNodeRemoveChild(parent->node, node);
		parent->visual.Children().Remove(visual);
		auto& siblings = parent->children;
		siblings.erase(std::remove(siblings.begin(), siblings.end(), this), siblings.end());
		auto& all = win->elements;
		all.erase(std::remove_if(all.begin(), all.end(), [this](const std::unique_ptr<Element>& ptr) {
			return ptr.get() == this;
			}));
	}

	void Element::removeChild(Element* ele)
	{
		ele->removeSelf();
	}

	void Element::insertChild(const int& index, Element* ele)
	{
		ele->parent = this;
		ele->initProperty();
		Composition::Visual v{ nullptr };
		int i = 0;
		for (auto child : visual.Children()) {
			if (i == index) {
				v = child;
				break;
			}
			i++;
		}
		visual.Children().InsertAbove(ele->visual, v);
		YGNodeInsertChild(node, ele->node, index);
	}

	void Element::addChild(Element* ele)
	{
		ele->parent = this;
		ele->initProperty();
		visual.Children().InsertAtTop(ele->visual);
		YGNodeInsertChild(node, ele->node, YGNodeGetChildCount(node));
		children.push_back(ele);
	}

	// 事件回调方法
	size_t Element::onMouseEnter(std::function<void(const MouseEvent&)> callback)
	{
		mouseEnterCBId += 1;
		mouseEnterCBs.insert({ mouseEnterCBId, callback });
		return mouseEnterCBId;
	}

	size_t Element::onMouseLeave(std::function<void(const MouseEvent&)> callback)
	{
		mouseLeaveCBId += 1;
		mouseLeaveCBs.insert({ mouseLeaveCBId, callback });
		return mouseLeaveCBId;
	}

	size_t Element::onMouseMove(std::function<void(const MouseEvent&)> callback)
	{
		mouseMoveCBId += 1;
		mouseMoveCBs.insert({ mouseMoveCBId, callback });
		return mouseMoveCBId;
	}

	size_t Element::onMouseDown(std::function<void(const MouseEvent&)> callback)
	{
		mouseDownCBId += 1;
		mouseDownCBs.insert({ mouseDownCBId, callback });
		return mouseDownCBId;
	}

	size_t Element::onMouseUp(std::function<void(const MouseEvent&)> callback)
	{
		mouseUpCBId += 1;
		mouseUpCBs.insert({ mouseUpCBId, callback });
		return mouseUpCBId;
	}

	void Element::offMouseEnter(const size_t& callbackId)
	{
		mouseEnterCBs.erase(callbackId);
	}

	void Element::offMouseLeave(const size_t& callbackId)
	{
		mouseLeaveCBs.erase(callbackId);
	}

	void Element::offMouseMove(const size_t& callbackId)
	{
		mouseMoveCBs.erase(callbackId);
	}

	void Element::offMouseDown(const size_t& callbackId)
	{
		mouseDownCBs.erase(callbackId);
	}

	void Element::offMouseUp(const size_t& callbackId)
	{
		mouseUpCBs.erase(callbackId);
	}

	// 事件触发方法
	void Element::mouseEnter(const MouseEvent& event)
	{
		if (!hoverBackgroundColor.isTransparent()) {  // 只有设置了才生效
			visual.Brush(win->compositor.CreateColorBrush(hoverBackgroundColor.getUIColor()));
		}
		for (const auto& pair : mouseEnterCBs) {
			pair.second(event);
		}
	}

	void Element::mouseLeave(const MouseEvent& event)
	{
		if (!hoverBackgroundColor.isTransparent()) {  // 只有设置了才生效
			visual.Brush(win->compositor.CreateColorBrush(backgroundColor.getUIColor()));
		}
		for (const auto& pair : mouseLeaveCBs) {
			pair.second(event);
		}
	}

	void Element::mouseMove(const MouseEvent& event)
	{
		for (const auto& pair : mouseMoveCBs) {
			pair.second(event);
		}
	}

	void Element::mouseDown(const MouseEvent& event)
	{
		for (const auto& pair : mouseDownCBs) {
			pair.second(event);
		}
	}

	void Element::mouseUp(const MouseEvent& event)
	{
		for (const auto& pair : mouseUpCBs) {
			pair.second(event);
		}
	}

	
	void Element::setWidth(const float& width)
	{
		YGNodeStyleSetWidth(node, width * win->dpi);
	}

	void Element::setHeight(const float& height)
	{
		YGNodeStyleSetHeight(node, height * win->dpi);
	}

	void Element::setWidthPercent(const float& widthPercent)
	{
		YGNodeStyleSetWidthPercent(node, widthPercent);
	}

	void Element::setHeightPercent(const float& heightPercent)
	{
		YGNodeStyleSetHeightPercent(node, heightPercent);
	}

	void Element::setMarginLeft(const float& marginLeft)
	{
		YGNodeStyleSetMargin(node, YGEdge::YGEdgeLeft, marginLeft * win->dpi);
	}

	void Element::setMarginTop(const float& marginTop)
	{
		YGNodeStyleSetMargin(node, YGEdge::YGEdgeTop, marginTop * win->dpi);
	}

	void Element::setMarginRight(const float& marginRight)
	{
		YGNodeStyleSetMargin(node, YGEdge::YGEdgeRight, marginRight * win->dpi);
	}

	void Element::setMarginBottom(const float& marginBottom)
	{
		YGNodeStyleSetMargin(node, YGEdge::YGEdgeBottom, marginBottom * win->dpi);
	}

	void Element::setPaddingLeft(const float& paddingLeft)
	{
		YGNodeStyleSetPadding(node, YGEdgeLeft, paddingLeft * win->dpi);
	}

	void Element::setPaddingTop(const float& paddingTop)
	{
		YGNodeStyleSetPadding(node, YGEdgeTop, paddingTop * win->dpi);
	}

	void Element::setPaddingRight(const float& paddingRight)
	{
		YGNodeStyleSetPadding(node, YGEdgeRight, paddingRight * win->dpi);
	}

	void Element::setPaddingBottom(const float& paddingBottom)
	{
		YGNodeStyleSetPadding(node, YGEdgeBottom, paddingBottom * win->dpi);
	}

	void Element::setFlexGrow(const float& flexGrow)
	{
		YGNodeStyleSetFlexGrow(node, flexGrow);
	}

	void Element::setFlexShrink(const float& flexShrink)
	{
		YGNodeStyleSetFlexShrink(node, flexShrink);
	}

	void Element::setWrap(const Wrap& wrap)
	{
		YGNodeStyleSetFlexWrap(node, (YGWrap)wrap);
	}

	void Element::setAlign(const Align& align)
	{
		YGNodeStyleSetAlignItems(node, (YGAlign)align);
	}

	void Element::setJustify(const Justify& justify)
	{
		YGNodeStyleSetJustifyContent(node, (YGJustify)justify);
	}

	void Element::setFlexDirection(const FlexDirection& flexDirection)
	{
		YGNodeStyleSetFlexDirection(node, (YGFlexDirection)flexDirection);
	}

}



