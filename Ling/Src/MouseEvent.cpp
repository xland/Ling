#include "../Include/MouseEvent.h"
#include "../Include/Element.h"

namespace Ling {
	MouseEvent::MouseEvent(const int& x, const int& y, const MouseButton& mouseBtn) :globalX{ x }, globalY{ y }, mouseBtn{ mouseBtn }
	{
	}

	MouseEvent::~MouseEvent()
	{
	}

	void MouseEvent::setRelativePosition(Element* ele)
	{
		x = this->globalX - ele->globalX;
		y = this->globalY - ele->globalY;
	}
}
