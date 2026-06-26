#pragma once
#include "MouseButton.h"

namespace Ling {
	class Element;
	class MouseEvent
	{
		public:
			MouseEvent(const int& x, const int& y, const MouseButton& mouseBtn = MouseButton::Left);
			~MouseEvent();
		public:
			int globalX, globalY, x{ 0 }, y{ 0 };
			MouseButton mouseBtn;
		protected:
		private:
	};
}

