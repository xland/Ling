#include "../Include/Style.h"

namespace Ling {
	void Style::setBorderWidth(const float& borderWidth)
	{
		this->borderWidth = borderWidth;
	}
	float Style::getBorderWidth()
	{
		return borderWidth;
	}
	void Style::setRadius(const float& radius)
	{
		radiusLT = radius;
		radiusRT = radius;
		radiusRB = radius;
		radiusLB = radius;
	}
	void Style::setRadius(const float& radiusLT, const float& radiusRT, const float& radiusRB, const float& radiusLB)
	{
		this->radiusLT = radiusLT;
		this->radiusRT = radiusRT;
		this->radiusRB = radiusRB;
		this->radiusLB = radiusLB;
	}
	std::array<float, 4> Style::getRadius()
	{
		return std::array<float, 4>{radiusLT, radiusRT, radiusRB, radiusLB};
	}
	float Style::getRadiusLT()
	{
		return radiusLT;
	}
	float Style::getRadiusRT()
	{
		return radiusRT;
	}
	float Style::getRadiusRB()
	{
		return radiusRB;
	}
	float Style::getRadiusLB()
	{
		return radiusLB;
	}
	void Style::setBackgroundColor(const Color& backgroundColor) 
	{
		this->backgroundColor = backgroundColor;
	}
	void Style::setBorderColor(const Color& borderColor) 
	{
		this->borderColor = borderColor;
	}
	Color& Style::getBackgroundColor()
	{
		return backgroundColor;
	}
	Color& Style::getBorderColor()
	{
		return borderColor;
	}
	void Style::setCaptionFlag(bool captionFlag)
	{
		this->captionFlag = captionFlag;
	}
	bool Style::getCaptionFlag()
	{
		return captionFlag;
	}
	void Style::setMargin(const float& val) 
	{
		marginL = val;
		marginT = val;
		marginR = val;
		marginB = val;
	}
	void Style::setMargin(const float& left, const float& top, const float& right, const float& bottom)
	{
		this->marginL = left;
		this->marginT = top;
		this->marginR = right;
		this->marginB = bottom;
	}
	float Style::getMarginL()
	{
		return marginL;
	}
	float Style::getMarginT()
	{
		return marginT;
	}
	float Style::getMarginR()
	{
		return marginR;
	}
	float Style::getMarginB()
	{
		return marginB;
	}
	void Style::setMarginL(const float& val)
	{
		marginL = val;
	}
	void Style::setMarginT(const float& val)
	{
		marginT = val;
	}
	void Style::setMarginR(const float& val)
	{
		marginR = val;
	}
	void Style::setMarginB(const float& val)
	{
		marginB = val;
	}
	void Style::setPadding(const float& val)
	{
		paddingL = val;
		paddingT = val;
		paddingR = val;
		paddingB = val;
	}
	void Style::setPadding(const float& left, const float& top, const float& right, const float& bottom)
	{
		paddingL = left;
		paddingT = top;
		paddingR = right;
		paddingB = bottom;
	}
	float Style::getPaddingL()
	{
		return paddingL;
	}
	float Style::getPaddingT()
	{
		return paddingT;
	}
	float Style::getPaddingR()
	{
		return paddingR;
	}
	float Style::getPaddingB()
	{
		return paddingB;
	}
	void Style::setPaddingL(const float& val)
	{
		paddingL = val;
	}
	void Style::setPaddingT(const float& val)
	{
		paddingT = val;
	}
	void Style::setPaddingR(const float& val)
	{
		paddingR = val;
	}
	void Style::setPaddingB(const float& val)
	{
		paddingB = val;
	}
}