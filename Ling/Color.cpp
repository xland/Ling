#include <include/core/SkColor.h>
#include "Color.h"

int getColorA(const Color& color)
{
	return SkColorGetA(color);
}
int getColorR(const Color& color)
{
	return SkColorGetR(color);
}
int getColorG(const Color& color)
{
	return SkColorGetG(color);
}
int getColorB(const Color& color)
{
	return SkColorGetB(color);
}
Color toColor(const int& r, const int& g, const int& b, const int& a)
{
	return SkColorSetARGB(a, r, g, b);
}
