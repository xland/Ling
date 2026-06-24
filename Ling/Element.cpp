#include "Element.h"
#include "WindowBase.h"
namespace Ling {
	Element::Element() : node{ YGNodeNew() }
	{
		YGNodeSetContext(node, this);
	}
	Element::~Element()
	{
		YGNodeFree(node);
	}

	void Element::layout()
	{
		x = YGNodeLayoutGetLeft(node);
		y = YGNodeLayoutGetTop(node);
		xAbs = parent->xAbs+x;
		yAbs = parent->yAbs+y;
		w = YGNodeLayoutGetWidth(node);
		h = YGNodeLayoutGetHeight(node);
		visual.Offset({ x, y, 0.0f });
		visual.Size({ w, h });
		for (auto& child : children) {
			child->layout();
		}
	}
	void Element::setBackgroundColor(const Color& backgroundColor)
	{
		this->backgroundColor = backgroundColor;
	}
	void Element::initProperty(const Composition::Compositor& comp)
	{
		parent = this;
		visual = comp.CreateSpriteVisual();
		visual.Brush(comp.CreateColorBrush(backgroundColor.getUIColor()));
		win = win;
	}
	void Element::insertChild(const int& index, const std::shared_ptr<Element>& ele)
	{
		ele->initProperty(visual.Compositor());
		Composition::Visual v{nullptr};
		int i = 0;
		for (auto child : visual.Children())
		{
			if (i == index) {
				v = child;
				break;
			}
			i++;
		}
		visual.Children().InsertAbove(ele->visual,v);
		YGNodeInsertChild(node, ele->node, index);
	}
	void Element::addChild(const std::shared_ptr<Element>& ele)
	{
		ele->initProperty(visual.Compositor());
		visual.Children().InsertAtTop(ele->visual);
		YGNodeInsertChild(node, ele->node, YGNodeGetChildCount(node));
		children.push_back(ele);
	}

	void Element::setWidth(const float& w)
	{
		YGNodeStyleSetWidth(node, w);
	}
	void Element::setHeight(const float& h)
	{
		YGNodeStyleSetHeight(node, h);
	}
	void Element::setSize(const float& w, const float& h)
	{
		YGNodeStyleSetWidth(node, w);
		YGNodeStyleSetHeight(node, h);
	}
	void Element::setWidthPercent(const float& percent)
	{
		YGNodeStyleSetWidthPercent(node, percent);
	}
	void Element::setHeightPercent(const float& percent)
	{
		YGNodeStyleSetHeightPercent(node, percent);
	}
	void Element::setSizePercent(const float& w, const float& h)
	{
		YGNodeStyleSetWidthPercent(node, w);
		YGNodeStyleSetHeightPercent(node, h);
	}
	void Element::setFlexGrow(const float& val)
	{
		YGNodeStyleSetFlexGrow(node, val);
	}

	void Element::setFlexShrink(const float& val)
	{
		YGNodeStyleSetFlexShrink(node, val);
	}
	float Element::getLeft()
	{
		return YGNodeLayoutGetLeft(node);
	}
	float Element::getTop()
	{
		return YGNodeLayoutGetTop(node);
	}
	float Element::getWidth()
	{
		return YGNodeLayoutGetWidth(node);
	}
	float Element::getHeight()
	{
		return YGNodeLayoutGetHeight(node);
	}

	void Element::setMargin(const float& val)
	{
		YGNodeStyleSetMargin(node, YGEdgeAll, val);
	}

	void Element::setMargin(const float& left, const float& top, const float& right, const float& bottom)
	{
		YGNodeStyleSetMargin(node, YGEdgeLeft, left);
		YGNodeStyleSetMargin(node, YGEdgeTop, top);
		YGNodeStyleSetMargin(node, YGEdgeRight, right);
		YGNodeStyleSetMargin(node, YGEdgeBottom, bottom);
	}
	float Element::getMarginLeft()
	{
		return YGNodeStyleGetMargin(node, YGEdge::YGEdgeLeft).value;
	}
	float Element::getMarginTop()
	{
		return YGNodeStyleGetMargin(node, YGEdge::YGEdgeTop).value;
	}
	float Element::getMarginRight()
	{
		return YGNodeStyleGetMargin(node, YGEdge::YGEdgeRight).value;
	}
	float Element::getMarginBottom()
	{
		return YGNodeStyleGetMargin(node, YGEdge::YGEdgeBottom).value;
	}
	void Element::setMarginLeft(const float& val)
	{
		YGNodeStyleSetMargin(node, YGEdge::YGEdgeLeft, val);
	}
	void Element::setMarginTop(const float& val)
	{
		YGNodeStyleSetMargin(node, YGEdge::YGEdgeTop, val);
	}
	void Element::setMarginRight(const float& val)
	{
		YGNodeStyleSetMargin(node, YGEdge::YGEdgeRight, val);
	}
	void Element::setMarginBottom(const float& val)
	{
		YGNodeStyleSetMargin(node, YGEdge::YGEdgeBottom, val);
	}

	void Element::setPadding(const float& val)
	{
		YGNodeStyleSetPadding(node, YGEdgeAll, val);
	}

	void Element::setPadding(const float& left, const float& top, const float& right, const float& bottom)
	{
		YGNodeStyleSetPadding(node, YGEdgeLeft, left);
		YGNodeStyleSetPadding(node, YGEdgeTop, top);
		YGNodeStyleSetPadding(node, YGEdgeRight, right);
		YGNodeStyleSetPadding(node, YGEdgeBottom, bottom);
	}

	void Element::setPaddingLeft(const float& val)
	{
		YGNodeStyleSetPadding(node, YGEdgeLeft, val);
	}

	void Element::setPaddingTop(const float& val)
	{
		YGNodeStyleSetPadding(node, YGEdgeTop, val);
	}

	void Element::setPaddingRight(const float& val)
	{
		YGNodeStyleSetPadding(node, YGEdgeRight, val);
	}

	void Element::setPaddingBottom(const float& val)
	{
		YGNodeStyleSetPadding(node, YGEdgeBottom, val);
	}

	float Element::getPaddingLeft()
	{
		return YGNodeStyleGetPadding(node, YGEdge::YGEdgeLeft).value;
	}

	float Element::getPaddingTop()
	{
		return YGNodeStyleGetPadding(node, YGEdge::YGEdgeTop).value;
	}

	float Element::getPaddingRight()
	{
		return YGNodeStyleGetPadding(node, YGEdge::YGEdgeRight).value;
	}

	float Element::getPaddingBottom()
	{
		return YGNodeStyleGetPadding(node, YGEdge::YGEdgeBottom).value;
	}
	void Element::setAlignItems(const Align& val)
	{
		YGNodeStyleSetAlignItems(node, (YGAlign)val);
	}

	void Element::setFlexWrap(const Wrap& val)
	{
		YGNodeStyleSetFlexWrap(node, (YGWrap)val);
	}
	void Element::setJustifyContent(const Justify& val)
	{
		YGNodeStyleSetJustifyContent(node, (YGJustify)val);
	}
	void Element::setFlexDirection(const FlexDirection& flexDirection)
	{
		YGNodeStyleSetFlexDirection(node, (YGFlexDirection)flexDirection);
	}



}