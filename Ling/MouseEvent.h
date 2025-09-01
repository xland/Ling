#pragma once
#include "MouseButton.h"
class Element;
class MouseEvent
{
public:
	MouseEvent(const int& x,const int& y,const MouseButton& mouseBtn = MouseButton::Left);
	~MouseEvent();
	void setRelativePosition(Element* ele);
public:
	int globalX, globalY, x{ 0 }, y{ 0 };
	MouseButton mouseBtn;
protected:
private:
};

