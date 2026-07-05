#include "pch.h"
#include "Element.h"
#include "WindowBase.h"
#include "Property.h"
namespace Ling {

	Element::Element(WindowBase* win) : win(win), node(YGNodeNew()), visual{ win->compositor.CreateSpriteVisual() }
	{
		YGNodeSetContext(node, this);
	}
	Element::~Element()
	{
		YGNodeFree(node);
	}
	void Element::setProperty(const std::shared_ptr<Property>& property)
	{
		if (this->property.get()) {
			this->property->elements.erase(std::remove(this->property->elements.begin(), this->property->elements.end(), this), this->property->elements.end());
		}
		this->property = property;
		this->property->elements.push_back(this);
		for (auto& [first,second] : property->dataFloat)
		{
			propertyChanged(first, &second);
		}
		for (auto& [first, second] : property->dataInt)
		{
			propertyChanged(first, &second);
		}
		for (auto& [first, second] : property->dataBool)
		{
			propertyChanged(first, &second);
		}
		for (auto& [first, second] : property->dataText)
		{
			propertyChanged(first, &second);
		}
		for (auto& [first, second] : property->dataColor)
		{
			propertyChanged(first, &second);
		}
	}
	void Element::propertyChanged(const Ling::PropertyType& key, const void* value)
	{
		if (key == PropertyType::Width) {
			setWidth(*(float*)value);
		}
		else if (key == PropertyType::Height) {
			setHeight(*(float*)value);
		}
		else if (key == PropertyType::WidthPercent) {
			setWidthPercent(*(float*)value);
		}
		else if (key == PropertyType::HeightPercent) {
			setHeightPercent(*(float*)value);
		}
		else if (key == PropertyType::MarginLeft) {
			setMarginLeft(*(float*)value);
		}
		else if (key == PropertyType::MarginTop) {
			setMarginTop(*(float*)value);
		}
		else if (key == PropertyType::MarginRight) {
			setMarginRight(*(float*)value);
		}
		else if (key == PropertyType::MarginBottom) {
			setMarginBottom(*(float*)value);
		}
		else if (key == PropertyType::PaddingLeft) {
			setPaddingLeft(*(float*)value);
		}
		else if (key == PropertyType::PaddingTop) {
			setPaddingTop(*(float*)value);
		}
		else if (key == PropertyType::PaddingRight) {
			setPaddingRight(*(float*)value);
		}
		else if (key == PropertyType::PaddingBottom) {
			setPaddingBottom(*(float*)value);
		}
		else if (key == PropertyType::FlexGrow) {
			setFlexGrow(*(float*)value);
		}
		else if (key == PropertyType::FlexShrink) {
			setFlexShrink(*(float*)value);
		}
		else if (key == PropertyType::Wrap) {
			setWrap(*(Ling::Wrap*)value);
		}
		else if (key == PropertyType::Justify) {
			setJustify(*(Ling::Justify*)value);
		}
		else if (key == PropertyType::Align) {
			setAlign(*(Ling::Align*)value);
		}
		else if (key == PropertyType::FlexDirection) {
			setFlexDirection(*(Ling::FlexDirection*)value);
		}
		else if (key == PropertyType::BackgroundColor) {
			setBackgroundColor(*(Color*)value);
		}
		else if (key == PropertyType::BackgroundHoverColor) {
			setBackgroundHoverColor(*(Color*)value);
		}
		else if (key == PropertyType::ForegroundColor) {
			setForegroundColor(*(Color*)value);
		}
		else if (key == PropertyType::ForegroundHoverColor) {
			setForegroundHoverColor(*(Color*)value);
		}
		else if (key == PropertyType::Visible) {
			setVisible(*(bool*)value);
		}
		else if (key == PropertyType::Text) {
			setText(*(std::wstring*)value);
		}
		else if (key == PropertyType::Cursor) {
			setCursor((HCURSOR)value);
		}
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
		if (property.get()) {
			if (property->hasCursor()) {
				SetCursor(property->getCursor());
				return true;
			}
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
		visual.Children().InsertAtTop(ele->visual);
		YGNodeInsertChild(node, ele->node, YGNodeGetChildCount(node));
		children.push_back(ele);
	}

	// 事件回调方法

	winrt::event_token Element::onMouseEnter(winrt::delegate<void(const MouseEvent&)> const& handler)
	{
		return eventMouseEnter.add(handler);
	}


	winrt::event_token Element::onMouseLeave(winrt::delegate<void(const MouseEvent&)> const& handler)
	{
		return eventMouseLeave.add(handler);
	}


	winrt::event_token Element::onMouseMove(winrt::delegate<void(const MouseEvent&)> const& handler)
	{
		return eventMouseMove.add(handler);
	}


	winrt::event_token Element::onMouseDown(winrt::delegate<void(const MouseEvent&)> const& handler)
	{
		return eventMouseDown.add(handler);
	}


	winrt::event_token Element::onMouseUp(winrt::delegate<void(const MouseEvent&)> const& handler)
	{
		return eventMouseUp.add(handler);
	}

	void Element::offMouseEnter(const winrt::event_token& callbackId)
	{
		eventMouseEnter.remove(callbackId);
	}

	void Element::offMouseLeave(const winrt::event_token& callbackId)
	{
		eventMouseLeave.remove(callbackId);
	}

	void Element::offMouseMove(const winrt::event_token& callbackId)
	{
		eventMouseMove.remove(callbackId);
	}

	void Element::offMouseDown(const winrt::event_token& callbackId)
	{
		eventMouseDown.remove(callbackId);
	}

	void Element::offMouseUp(const winrt::event_token& callbackId)
	{
		eventMouseUp.remove(callbackId);
	}

	// 事件触发方法
	void Element::mouseEnter(const MouseEvent& event)
	{
		auto& hoverBackgroundColor = property->getColorBackgroundHover();
		if (!hoverBackgroundColor.isTransparent()) {  // 只有设置了才生效
			visual.Brush(win->compositor.CreateColorBrush(hoverBackgroundColor.getUIColor()));
		}
		isMouseEnter = true;
		eventMouseEnter(event);
	}

	void Element::mouseLeave(const MouseEvent& event)
	{
		auto& backgroundColor = property->getColorBackground();
		if (!backgroundColor.isTransparent()) {  // 只有设置了才生效
			visual.Brush(win->compositor.CreateColorBrush(backgroundColor.getUIColor()));
		}
		isMouseEnter = false;
		eventMouseLeave(event);
	}

	void Element::mouseMove(const MouseEvent& event)
	{
		eventMouseMove(event);
	}

	void Element::mouseDown(const MouseEvent& event)
	{
		eventMouseDown(event);
	}

	void Element::mouseUp(const MouseEvent& event)
	{
		eventMouseUp(event);
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

	void Element::setBackgroundColor(const Color& color)
	{
		//if (!isMouseEnter || property->getColorBackgroundHover().isTransparent()) {
		//	visual.Brush(win->compositor.CreateColorBrush(color.getUIColor()));
		//}
	}

	void Element::setBackgroundHoverColor(const Color& color)
	{
		//if (isMouseEnter) {
		//	visual.Brush(win->compositor.CreateColorBrush(color.getUIColor()));
		//}
	}

	void Element::setForegroundColor(const Color& color)
	{
	}

	void Element::setForegroundHoverColor(const Color& color)
	{
	}

	void Element::setVisible(const bool& flag)
	{
	}

	void Element::setText(const std::wstring& text)
	{
	}

	void Element::setCursor(const HCURSOR cursor)
	{
	}

}



