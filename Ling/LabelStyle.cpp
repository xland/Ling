#include "pch.h"
#include "LabelStyle.h"
#include "Label.h"

namespace Ling {
	LabelStyle::LabelStyle()
	{
	}
	LabelStyle::~LabelStyle()
	{
	}
	void LabelStyle::setForegroundColor(const Color& color)
	{
		this->foregroundColor = color;
		for (auto ele : elements)
		{
			((Label*)ele)->setForegroundColor(color);
		}
	}
	void LabelStyle::setForegroundHoverColor(const Color& color)
	{
		this->foregroundHoverColor = color;
		for (auto ele : elements)
		{
			((Label*)ele)->setForegroundHoverColor(color);
		}
	}
	void LabelStyle::setFontSize(const float& fontSize)
	{
		this->fontSize = fontSize;
		for (auto ele : elements)
		{
			((Label*)ele)->setFontSize(fontSize);
		}
	}
	void LabelStyle::setFontFamily(const std::wstring& fontFamily)
	{
		this->fontFamily = fontFamily;
		for (auto ele : elements)
		{
			((Label*)ele)->setFontFamily(fontFamily);
		}
	}
	const std::wstring& LabelStyle::getFontFamily()
	{
		return fontFamily;
	}
	const float& LabelStyle::getFontSize()
	{
		return fontSize;
	}
	const Color& LabelStyle::getForegroundColor()
	{
		return foregroundColor;
	}
	const Color& LabelStyle::getForegroundHoverColor()
	{
		return foregroundHoverColor;
	}
}