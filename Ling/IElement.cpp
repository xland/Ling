#include "pch.h"
#include "IElement.h"
#include "WindowBase.h"
namespace Ling {

	IElement::IElement(WindowBase* win) : win(win), node(YGNodeNew()), visual{ win->compositor.CreateSpriteVisual() }
	{
		YGNodeSetContext(node, this);
	}
	IElement::~IElement()
	{
		YGNodeFree(node);
	}
	bool IElement::isAncestor(const IElement* target)
	{
		const IElement* current = target;
		while (current) {
			if (current == this) return true;
			current = current->parent;
		}
		return false;
	}

	IElement* IElement::findAncestor(IElement* other)
	{
		std::unordered_set<IElement*> ancestors;
		IElement* current = this;
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

	void IElement::layout()
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

	bool IElement::setCursor()
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

	IElement* IElement::hitTest(const int& x, const int& y)
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

	void IElement::removeSelf()
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
		all.erase(std::remove_if(all.begin(), all.end(), [this](const std::unique_ptr<IElement>& ptr) {
			return ptr.get() == this;
			}));
	}

	void IElement::removeChild(IElement* ele)
	{
		ele->removeSelf();
	}

	void IElement::insertChild(const int& index, IElement* ele)
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

	void IElement::addChild(IElement* ele)
	{
		ele->parent = this;
		ele->initProperty();
		visual.Children().InsertAtTop(ele->visual);
		YGNodeInsertChild(node, ele->node, YGNodeGetChildCount(node));
		children.push_back(ele);
	}

	// 事件回调方法
	size_t IElement::onMouseEnter(std::function<void(const MouseEvent&)> callback)
	{
		mouseEnterCBId += 1;
		mouseEnterCBs.insert({ mouseEnterCBId, callback });
		return mouseEnterCBId;
	}

	size_t IElement::onMouseLeave(std::function<void(const MouseEvent&)> callback)
	{
		mouseLeaveCBId += 1;
		mouseLeaveCBs.insert({ mouseLeaveCBId, callback });
		return mouseLeaveCBId;
	}

	size_t IElement::onMouseMove(std::function<void(const MouseEvent&)> callback)
	{
		mouseMoveCBId += 1;
		mouseMoveCBs.insert({ mouseMoveCBId, callback });
		return mouseMoveCBId;
	}

	size_t IElement::onMouseDown(std::function<void(const MouseEvent&)> callback)
	{
		mouseDownCBId += 1;
		mouseDownCBs.insert({ mouseDownCBId, callback });
		return mouseDownCBId;
	}

	size_t IElement::onMouseUp(std::function<void(const MouseEvent&)> callback)
	{
		mouseUpCBId += 1;
		mouseUpCBs.insert({ mouseUpCBId, callback });
		return mouseUpCBId;
	}

	void IElement::offMouseEnter(const size_t& callbackId)
	{
		mouseEnterCBs.erase(callbackId);
	}

	void IElement::offMouseLeave(const size_t& callbackId)
	{
		mouseLeaveCBs.erase(callbackId);
	}

	void IElement::offMouseMove(const size_t& callbackId)
	{
		mouseMoveCBs.erase(callbackId);
	}

	void IElement::offMouseDown(const size_t& callbackId)
	{
		mouseDownCBs.erase(callbackId);
	}

	void IElement::offMouseUp(const size_t& callbackId)
	{
		mouseUpCBs.erase(callbackId);
	}

	// 事件触发方法
	void IElement::mouseEnter(const MouseEvent& event)
	{
		if (!hoverBackgroundColor.isTransparent()) {  // 只有设置了才生效
			visual.Brush(win->compositor.CreateColorBrush(hoverBackgroundColor.getUIColor()));
		}
		for (const auto& pair : mouseEnterCBs) {
			pair.second(event);
		}
	}

	void IElement::mouseLeave(const MouseEvent& event)
	{
		if (!hoverBackgroundColor.isTransparent()) {  // 只有设置了才生效
			visual.Brush(win->compositor.CreateColorBrush(backgroundColor.getUIColor()));
		}
		for (const auto& pair : mouseLeaveCBs) {
			pair.second(event);
		}
	}

	void IElement::mouseMove(const MouseEvent& event)
	{
		for (const auto& pair : mouseMoveCBs) {
			pair.second(event);
		}
	}

	void IElement::mouseDown(const MouseEvent& event)
	{
		for (const auto& pair : mouseDownCBs) {
			pair.second(event);
		}
	}

	void IElement::mouseUp(const MouseEvent& event)
	{
		for (const auto& pair : mouseUpCBs) {
			pair.second(event);
		}
	}
	// get 方法
	float IElement::getLeft()
	{
		return YGNodeLayoutGetLeft(node);
	}

	float IElement::getTop()
	{
		return YGNodeLayoutGetTop(node);
	}

	float IElement::getWidth()
	{
		return YGNodeLayoutGetWidth(node);
	}

	float IElement::getHeight()
	{
		return YGNodeLayoutGetHeight(node);
	}

	float IElement::getMarginLeft()
	{
		return YGNodeStyleGetMargin(node, YGEdge::YGEdgeLeft).value;
	}

	float IElement::getMarginTop()
	{
		return YGNodeStyleGetMargin(node, YGEdge::YGEdgeTop).value;
	}

	float IElement::getMarginRight()
	{
		return YGNodeStyleGetMargin(node, YGEdge::YGEdgeRight).value;
	}

	float IElement::getMarginBottom()
	{
		return YGNodeStyleGetMargin(node, YGEdge::YGEdgeBottom).value;
	}

	float IElement::getPaddingLeft()
	{
		return YGNodeStyleGetPadding(node, YGEdge::YGEdgeLeft).value;
	}

	float IElement::getPaddingTop()
	{
		return YGNodeStyleGetPadding(node, YGEdge::YGEdgeTop).value;
	}

	float IElement::getPaddingRight()
	{
		return YGNodeStyleGetPadding(node, YGEdge::YGEdgeRight).value;
	}

	float IElement::getPaddingBottom()
	{
		return YGNodeStyleGetPadding(node, YGEdge::YGEdgeBottom).value;
	}
}



