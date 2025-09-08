#include <thorvg.h>
#include <cstdint>
#include <yoga/Yoga.h>
#include "../Include/Element.h"
#include "../Include/WindowBase.h"
#include "../Include/App.h"
namespace Ling {
	Element::Element() :shape{ tvg::Shape::gen() }
	{
	}

	Element::~Element()
	{
	}
	WindowBase* Element::getWindow()
	{
		return win;
	}

	void Element::setWindow(WindowBase* win)
	{
		win->scene->push(shape);
		this->win = win;
	}

	void Element::layout()
	{
		float x = getLeft();
		float y = getTop();
		if (parent) {
			globalX = x + parent->globalX;
			globalY = y + parent->globalY;
		}
		float w = getWidth();
		float h = getHeight();
		shape->reset();
		shape->appendRect(globalX, globalY, w, h, radius, radius);
	}

	void Element::update()
	{
		int left = (int)getGlobalX();
		int top = (int)getGlobalY();
		int right = (int)getWidth() + left;
		int bottom = (int)getHeight() + top;
		RECT r{ .left{left},.top{top}, .right{right}, .bottom{bottom} };
		auto hwnd = getWindow()->getHandle();
		InvalidateRect(hwnd, &r, false);
	}

	void Element::setParent(Element* ele)
	{
		this->parent = ele;
	}

	Element* Element::getParent()
	{
		return parent;
	}

	void Element::setBorderWidth(const float& borderWidth)
	{
		shape->strokeWidth(borderWidth);
	}
	float Element::getBorderWidth()
	{
		return shape->strokeWidth();
	}
	void Element::setRadius(const float& radius)
	{
		this->radius = radius;
	}
	float Element::getRadius()
	{
		return radius;
	}
	void Element::setBackgroundColor(const Color& backgroundColor)
	{
		shape->fill(backgroundColor.getR(), backgroundColor.getG(), backgroundColor.getB(), backgroundColor.getA());
	}
	void Element::setBorderColor(const Color& borderColor)
	{
		shape->strokeFill(borderColor.getR(), borderColor.getG(), borderColor.getB(), borderColor.getA());
	}
	void Element::setCaptionFlag(bool captionFlag)
	{
		this->captionFlag = captionFlag;
	}
	bool Element::getCaptionFlag()
	{
		return captionFlag;
	}
	void Element::setCursor(const Cursor& cursor)
	{
		this->cursor = cursor;
	}
	Cursor Element::getCursor() 
	{
		return cursor;
	}
}


