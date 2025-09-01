//#include <include/core/SkColor.h>
#include "../Include/Color.h"

namespace Ling {
	int getColorA(const Color& color)
	{
		return (color >> 24) & 0xFF;
	}
	int getColorR(const Color& color)
	{
		return (color >> 16) & 0xFF;
	}
	int getColorG(const Color& color)
	{
		return (color >> 8) & 0xFF;
	}
	int getColorB(const Color& color)
	{
		return color & 0xFF;
	}
	Color toColor(const int& r, const int& g, const int& b, const int& a)
	{
		return (static_cast<Color>(a) << 24) | (static_cast<Color>(r) << 16) |
			(static_cast<Color>(g) << 8) | (static_cast<Color>(b));
	}
}

