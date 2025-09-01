#include "MouseEvent.h"
#include "Element.h"

MouseEvent::MouseEvent(const int& x, const int& y, const MouseButton& mouseBtn) :globalX{ x }, globalY{ y }, mouseBtn{ mouseBtn }
{
}

MouseEvent::~MouseEvent()
{
}

void MouseEvent::setRelativePosition(Element* ele)
{
	x = globalX - ele->globalX;
	y = globalY - ele->globalY;
}
