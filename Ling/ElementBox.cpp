#include <yoga/Yoga.h>
#include <include/core/SkCanvas.h>
#include "ElementBox.h"

ElementBox::ElementBox()
{
}

ElementBox::~ElementBox()
{
}

void ElementBox::addChild(Element* ele)
{
	ele->parent = this;
	YGNodeInsertChild(node, ele->node, YGNodeGetChildCount(node));
	children.push_back(ele);
}

void ElementBox::insertChild(const int& index, Element* ele)
{
	ele->parent = this;
	YGNodeInsertChild(node, ele->node, index);
	children.insert(children.begin() + index, ele);
}


void ElementBox::paint(SkCanvas* canvas)
{
	Element::paint(canvas);
	float x = YGNodeLayoutGetLeft(node);
	float y = YGNodeLayoutGetTop(node);
	for (size_t i = 0; i < children.size(); i++)
	{
		canvas->save();
		canvas->translate(x, y);
		children[i]->paint(canvas);
		canvas->restore();
	}
}

std::vector<Element*>* ElementBox::getChildren()
{
	return &children;
}

void ElementBox::setAlignItems(const Align& val)
{
	YGNodeStyleSetAlignItems(node, (YGAlign)val);
}

void ElementBox::setJustifyContent(const Justify& val)
{
	YGNodeStyleSetJustifyContent(node, (YGJustify)val);
}
void ElementBox::setFlexDirection(const FlexDirection& flexDirection)
{
	YGNodeStyleSetFlexDirection(node, (YGFlexDirection)flexDirection);
}
void ElementBox::calculateGlobalPos(std::vector<Element*>* children)
{
	for (auto& child : *children)
	{
		child->globalX = YGNodeLayoutGetLeft(child->node) + child->parent->globalX;
		child->globalY = YGNodeLayoutGetTop(child->node) + child->parent->globalY;
		auto children = child->getChildren();
		if (children && children->size() > 0) {
			calculateGlobalPos(children);
		}
	}
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
	YGNodeCalculateLayout(node, YGUndefined, YGUndefined, YGDirectionLTR);
	calculateGlobalPos(&children);
}