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

		if (backgroundGradient) {
			if (backgroundGradient->getGradientType() == GradientType::Linear) {
				auto lFill = (tvg::LinearGradient*)backgroundGradient->getFill();
				lFill->linear(globalX, globalY, globalX + w, globalY + h);
			}
		}
	}
	void Element::update()
	{
		//int left = (int)getGlobalX()-1;
		//int top = (int)getGlobalY()-1;
		//int right = (int)getWidth() + left+1;
		//int bottom = (int)getHeight() + top+1;
		//RECT r{ .left{left},.top{top}, .right{right}, .bottom{bottom} };
		auto hwnd = getWindow()->getHandle();
		InvalidateRect(hwnd, nullptr, false);
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
	void Element::setBackgroundColor(const std::shared_ptr<Gradient>& backgroundGradient)
	{
		this->backgroundGradient = backgroundGradient;
		auto fill = backgroundGradient->getFill();
		std::vector<tvg::Fill::ColorStop> colors;
		auto data = backgroundGradient->getData();
		for (auto& pair:*data)
		{
			colors.push_back({ pair.first,
				pair.second.getR(),
				pair.second.getG(), 
				pair.second.getB(), 
				pair.second.getA() 
			});
		}
		fill->colorStops(colors.data(), colors.size());
		shape->fill(fill);
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


