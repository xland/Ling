#include <yoga/Yoga.h>
#include <thorvg.h>
#include "../Include/ElementBox.h"
namespace Ling {
	ElementBox::ElementBox()
	{
	}

	ElementBox::~ElementBox()
	{
	}

	void ElementBox::addChild(const std::shared_ptr<Element>& ele)
	{
		ele->setParent(this);
		Node::addChild(ele.get());
		children.push_back(ele);
	}

	void ElementBox::insertChild(const int& index, const std::shared_ptr<Element>& ele)
	{
		ele->setParent(this);
		Node::insertChild(ele.get(), index);
		children.insert(children.begin() + index, ele);
	}

	std::vector<std::shared_ptr<Element>>* ElementBox::getChildren()
	{
		return &children;
	}
	void ElementBox::setPadding(const float& val)
	{
		YGNodeStyleSetPadding(node, YGEdgeAll, val);
	}

	void ElementBox::setPadding(const float& left, const float& top, const float& right, const float& bottom)
	{
		YGNodeStyleSetPadding(node, YGEdgeLeft, left);
		YGNodeStyleSetPadding(node, YGEdgeTop, top);
		YGNodeStyleSetPadding(node, YGEdgeRight, right);
		YGNodeStyleSetPadding(node, YGEdgeBottom, bottom);
	}

	void ElementBox::setPaddingLeft(const float& val)
	{
		YGNodeStyleSetPadding(node, YGEdgeLeft, val);
	}

	void ElementBox::setPaddingTop(const float& val)
	{
		YGNodeStyleSetPadding(node, YGEdgeTop, val);
	}

	void ElementBox::setPaddingRight(const float& val)
	{
		YGNodeStyleSetPadding(node, YGEdgeRight, val);
	}

	void ElementBox::setPaddingBottom(const float& val)
	{
		YGNodeStyleSetPadding(node, YGEdgeBottom, val);
	}

	float ElementBox::getPaddingLeft()
	{
		return YGNodeStyleGetPadding(node, YGEdge::YGEdgeLeft).value;
	}

	float ElementBox::getPaddingTop()
	{
		return YGNodeStyleGetPadding(node, YGEdge::YGEdgeTop).value;
	}

	float ElementBox::getPaddingRight()
	{
		return YGNodeStyleGetPadding(node, YGEdge::YGEdgeRight).value;
	}

	float ElementBox::getPaddingBottom()
	{
		return YGNodeStyleGetPadding(node, YGEdge::YGEdgeBottom).value;
	}
	void ElementBox::setAlignItems(const Align& val)
	{
		YGNodeStyleSetAlignItems(node, (YGAlign)val);
	}

	void ElementBox::setFlexWrap(const Wrap& val)
	{
		YGNodeStyleSetFlexWrap(node, (YGWrap)val);
	}
	void ElementBox::setJustifyContent(const Justify& val)
	{
		YGNodeStyleSetJustifyContent(node, (YGJustify)val);
	}
	void ElementBox::setFlexDirection(const FlexDirection& flexDirection)
	{
		YGNodeStyleSetFlexDirection(node, (YGFlexDirection)flexDirection);
	}
	void ElementBox::casecadeShown()
	{
		shown();
		for (auto& child : children)
		{
			auto box = dynamic_cast<ElementBox*>(child.get());
			if (box) {
				box->casecadeShown();
			}
			else {
				child->shown();
			}
		}
	}
	void ElementBox::layout()
	{
		Element::layout();
		for (auto& child : children)
		{
			child->layout();
		}
	}
}