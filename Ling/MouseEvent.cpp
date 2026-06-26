#include "MouseEvent.h"
#include "Element.h"

namespace Ling {
	MouseEvent::MouseEvent(const int& x, const int& y, const MouseButton& mouseBtn) :globalX{ x }, globalY{ y }, mouseBtn{ mouseBtn }
	{
	}

	MouseEvent::~MouseEvent()
	{
	}
}
