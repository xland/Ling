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

	void ElementBox::addChild(Element* ele)
	{
		ele->setParent(this);
		Node::addChild(ele);
		children.push_back(ele);
	}

	void ElementBox::insertChild(const int& index, Element* ele)
	{
		ele->setParent(this);
		Node::insertChild(ele, index);
		children.insert(children.begin() + index, ele);
	}

	std::vector<Element*>* ElementBox::getChildren()
	{
		return &children;
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
			auto box = dynamic_cast<ElementBox*>(child);
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