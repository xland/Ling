#pragma once
#include "IElement.h"

namespace Ling {
	class WindowBase;
	template <typename Derived>
	class Element : public IElement
	{
	public:
		Element(WindowBase* win) :IElement(win) {}
		virtual ~Element() {}
		Derived* setBackgroundColor(const Color& color) {
			backgroundColor = color;
			return static_cast<Derived*>(this);
		}
		Derived* setHoverBackgroundColor(const Color& color)
		{
			hoverBackgroundColor = color;
			return static_cast<Derived*>(this);
		}
		Derived* setCursor(LPCWSTR cursor)
		{
			this->cursor = LoadCursor(NULL, cursor);
			return static_cast<Derived*>(this);
		}
		Derived* setWidth(const float& w)
		{
			YGNodeStyleSetWidth(node, w * win->dpi);
			return static_cast<Derived*>(this);
		}
		Derived* setHeight(const float& h) {
			YGNodeStyleSetHeight(node, h * win->dpi);
			return static_cast<Derived*>(this);
		}
		Derived* setSize(const float& w, const float& h) {
			setWidth(w);
			setHeight(h);
			return static_cast<Derived*>(this);
		}
		Derived* setWidthPercent(const float& percent) {
			YGNodeStyleSetWidthPercent(node, percent);
			return static_cast<Derived*>(this);
		}
		Derived* setHeightPercent(const float& percent) {
			YGNodeStyleSetHeightPercent(node, percent);
			return static_cast<Derived*>(this);
		}
		Derived* setSizePercent(const float& w, const float& h){
			setWidthPercent(w);
			setHeightPercent(h);
			return static_cast<Derived*>(this);
		}
		Derived* setFlexGrow(const float& val) {
			YGNodeStyleSetFlexGrow(node, val);
			return static_cast<Derived*>(this);
		}
		Derived* setFlexShrink(const float& val) {
			YGNodeStyleSetFlexShrink(node, val);
			return static_cast<Derived*>(this);
		}
		Derived* setMargin(const float& val) {
			setMargin(val, val, val, val);
			return static_cast<Derived*>(this);
		}
		Derived* setMargin(const float& left, const float& top, const float& right, const float& bottom) {
			setMarginLeft(left);setMarginTop(top);setMarginRight(right);setMarginBottom(bottom);
			return static_cast<Derived*>(this);
		}
		Derived* setMarginLeft(const float& val) {
			YGNodeStyleSetMargin(node, YGEdge::YGEdgeLeft, val * win->dpi);
			return static_cast<Derived*>(this);
		}
		Derived* setMarginTop(const float& val) {
			YGNodeStyleSetMargin(node, YGEdge::YGEdgeTop, val * win->dpi);
			return static_cast<Derived*>(this);
		}
		Derived* setMarginRight(const float& val) {
			YGNodeStyleSetMargin(node, YGEdge::YGEdgeRight, val * win->dpi);
			return static_cast<Derived*>(this);
		}
		Derived* setMarginBottom(const float& val) {
			YGNodeStyleSetMargin(node, YGEdge::YGEdgeBottom, val * win->dpi);
			return static_cast<Derived*>(this);
		}
		Derived* setPadding(const float& val) {
			setPadding(val, val, val, val);
			return static_cast<Derived*>(this);
		}
		Derived* setPadding(const float& left, const float& top, const float& right, const float& bottom) {
			setPaddingLeft(left);
			setPaddingTop(top);
			setPaddingRight(right);
			setPaddingBottom(bottom);
			return static_cast<Derived*>(this);
		}
		Derived* setPaddingLeft(const float& val) {
			YGNodeStyleSetPadding(node, YGEdgeLeft, val * win->dpi);
			return static_cast<Derived*>(this);
		}
		Derived* setPaddingTop(const float& val) {
			YGNodeStyleSetPadding(node, YGEdgeTop, val * win->dpi);
			return static_cast<Derived*>(this);
		}
		Derived* setPaddingRight(const float& val) {
			YGNodeStyleSetPadding(node, YGEdgeRight, val * win->dpi);
			return static_cast<Derived*>(this);
		}
		Derived* setPaddingBottom(const float& val) {
			YGNodeStyleSetPadding(node, YGEdgeBottom, val * win->dpi);
			return static_cast<Derived*>(this);
		}
		Derived* setFlexWrap(const Wrap& val) {
			YGNodeStyleSetFlexWrap(node, (YGWrap)val);
			return static_cast<Derived*>(this);
		}
		Derived* setAlignItems(const Align& val) {
			YGNodeStyleSetAlignItems(node, (YGAlign)val);
			return static_cast<Derived*>(this);
		}
		Derived* setJustifyContent(const Justify& val) {
			YGNodeStyleSetJustifyContent(node, (YGJustify)val);
			return static_cast<Derived*>(this);
		}
		Derived* setFlexDirection(const FlexDirection& flexDirection) {
			YGNodeStyleSetFlexDirection(node, (YGFlexDirection)flexDirection);
			return static_cast<Derived*>(this);
		}
	};
}