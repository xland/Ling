#include "pch.h"
#include "ElementStyle.h"
#include "Element.h"

namespace Ling {
	ElementStyle::ElementStyle()
	{
	}
	ElementStyle::~ElementStyle()
	{
	}
	void ElementStyle::setVisible(bool flag)
	{
		visible = flag;
		for (auto ele:elements)
		{
			ele->setVisible(flag);
		}
	}
	void ElementStyle::setCursor(const HCURSOR cursor)
	{
		this->cursor = cursor;
		for (auto ele : elements)
		{
			ele->setCursor(cursor);
		}
	}
	void ElementStyle::setWidth(const float& width)
	{
		this->width = width;
		for (auto ele : elements)
		{
			ele->setCursor(cursor);
		}
	}
	void ElementStyle::setHeight(const float& height)
	{
		this->height = height;
		for (auto ele : elements)
		{
			ele->setHeight(height);
		}
	}
	void ElementStyle::setSize(const float& width, const float& height)
	{
		this->width = width;
		this->height = height;
	}
	void ElementStyle::setWidthPercent(const float& widthPercent)
	{
		this->widthPercent = widthPercent;
		for (auto ele : elements)
		{
			ele->setWidthPercent(widthPercent);
		}
	}
	void ElementStyle::setHeightPercent(const float& heightPercent)
	{
		this->heightPercent = heightPercent;
		for (auto ele : elements)
		{
			ele->setHeightPercent(heightPercent);
		}
	}
	void ElementStyle::setSizePercent(const float& widthPercent, const float& heightPercent)
	{
		this->widthPercent = widthPercent;
		this->heightPercent = heightPercent;
	}
	void ElementStyle::setMarginLeft(const float& marginLeft)
	{
		this->marginLeft = marginLeft;
		for (auto ele : elements)
		{
			ele->setMarginLeft(marginLeft);
		}
	}
	void ElementStyle::setMarginTop(const float& marginTop)
	{
		this->marginTop = marginTop;
		for (auto ele : elements)
		{
			ele->setMarginTop(marginTop);
		}
	}
	void ElementStyle::setMarginRight(const float& marginRight)
	{
		this->marginRight = marginRight;
		for (auto ele : elements)
		{
			ele->setMarginRight(marginRight);
		}
	}
	void ElementStyle::setMarginBottom(const float& marginBottom)
	{
		this->marginBottom = marginBottom;
		for (auto ele : elements)
		{
			ele->setMarginBottom(marginBottom);
		}
	}
	void ElementStyle::setMargin(const float& margin)
	{
		this->marginLeft = margin;
		this->marginTop = margin;
		this->marginRight = margin;
		this->marginBottom = margin;
	}
	void ElementStyle::setMargin(const float& left, const float& top, const float& right, const float& bottom)
	{
		this->marginLeft = left;
		this->marginTop = top;
		this->marginRight = right;
		this->marginBottom = bottom;
	}
	void ElementStyle::setPaddingLeft(const float& paddingLeft)
	{
		this->paddingLeft = paddingLeft;
		for (auto ele : elements)
		{
			ele->setPaddingLeft(paddingLeft);
		}
	}
	void ElementStyle::setPaddingTop(const float& paddingTop)
	{
		this->paddingTop = paddingTop;
		for (auto ele : elements)
		{
			ele->setPaddingTop(paddingTop);
		}
	}
	void ElementStyle::setPaddingRight(const float& paddingRight)
	{
		this->paddingRight = paddingRight;
		for (auto ele : elements)
		{
			ele->setPaddingRight(paddingRight);
		}
	}
	void ElementStyle::setPaddingBottom(const float& paddingBottom)
	{
		this->paddingBottom = paddingBottom;
		for (auto ele : elements)
		{
			ele->setPaddingBottom(paddingBottom);
		}
	}
	void ElementStyle::setPadding(const float& padding)
	{
		this->paddingLeft = padding;
		this->paddingTop = padding;
		this->paddingRight = padding;
		this->paddingBottom = padding;
	}
	void ElementStyle::setPadding(const float& left, const float& top, const float& right, const float& bottom)
	{
		this->paddingLeft = left;
		this->paddingTop = top;
		this->paddingRight = right;
		this->paddingBottom = bottom;
	}
	void ElementStyle::setFlexGrow(const float& flexGrow)
	{
		this->flexGrow = flexGrow;
		for (auto ele : elements)
		{
			ele->setFlexGrow(flexGrow);
		}
	}
	void ElementStyle::setFlexShrink(const float& flexShrink)
	{
		this->flexShrink = flexShrink;
		for (auto ele : elements)
		{
			ele->setFlexShrink(flexShrink);
		}
	}
	void ElementStyle::setWrap(const Wrap& wrap)
	{
		this->wrap = wrap;
		for (auto ele : elements)
		{
			ele->setWrap(wrap);
		}
	}
	void ElementStyle::setAlign(const Align& align)
	{
		this->align = align;
		for (auto ele : elements)
		{
			ele->setAlign(align);
		}
	}
	void ElementStyle::setJustify(const Justify& justify)
	{
		this->justify = justify;
		for (auto ele : elements)
		{
			ele->setJustify(justify);
		}
	}
	void ElementStyle::setFlexDirection(const FlexDirection& flexDirection)
	{
		this->flexDirection = flexDirection;
		for (auto ele : elements)
		{
			ele->setFlexDirection(flexDirection);
		}
	}
	void ElementStyle::setBackgroundColor(const Color& color)
	{
		this->backgroundColor = color;
		for (auto ele : elements)
		{
			ele->setBackgroundColor(color);
		}
	}
	void ElementStyle::setBackgroundHoverColor(const Color& color)
	{
		this->backgroundHoverColor = color;
		for (auto ele : elements)
		{
			ele->setBackgroundHoverColor(color);
		}
	}

	bool ElementStyle::getVisible()
	{
		return visible;
	}
	HCURSOR ElementStyle::getCursor()
	{
		return cursor;
	}
	float ElementStyle::getWidth()
	{
		return width;
	}
	float ElementStyle::getHeight()
	{
		return height;
	}
	float ElementStyle::getWidthPercent()
	{
		return widthPercent;
	}
	float ElementStyle::getHeightPercent()
	{
		return heightPercent;
	}
	float ElementStyle::getMarginLeft()
	{
		return marginLeft;
	}
	float ElementStyle::getMarginTop()
	{
		return marginTop;
	}
	float ElementStyle::getMarginRight()
	{
		return marginRight;
	}
	float ElementStyle::getMarginBottom()
	{
		return marginBottom;
	}
	float ElementStyle::getPaddingLeft()
	{
		return paddingLeft;
	}
	float ElementStyle::getPaddingTop()
	{
		return paddingTop;
	}
	float ElementStyle::getPaddingRight()
	{
		return paddingRight;
	}
	float ElementStyle::getPaddingBottom()
	{
		return paddingBottom;
	}
	float ElementStyle::getFlexGrow()
	{
		return flexGrow;
	}
	float ElementStyle::getFlexShrink()
	{
		return flexShrink;
	}
	Wrap ElementStyle::getWrap()
	{
		return wrap;
	}
	Align ElementStyle::getAlign()
	{
		return align;
	}
	Justify ElementStyle::getJustify()
	{
		return justify;
	}
	FlexDirection ElementStyle::getFlexDirection()
	{
		return flexDirection;
	}
	Color ElementStyle::getColorBackground()
	{
		return backgroundColor;
	}
	Color ElementStyle::getBackgroundHoverColor()
	{
		return backgroundHoverColor;
	}

}


