#include "pch.h"
#include "Property.h"
#include "Element.h"
namespace Ling {

	Property::Property()
	{
	}

	Property::~Property()
	{
	}

	void Property::setWidth(const float& width)
	{
		setFloat(PropertyType::Width, width);
	}

	void Property::setHeight(const float& height)
	{
		setFloat(PropertyType::Height, height);
	}

	void Property::setSize(const float& width, const float& height)
	{
		setWidth(width);
		setHeight(height);
	}

	void Property::setWidthPercent(const float& widthPercent)
	{
		setFloat(PropertyType::WidthPercent, widthPercent);
	}

	void Property::setHeightPercent(const float& heightPercent)
	{
		setFloat(PropertyType::WidthPercent, heightPercent);
	}

	void Property::setSizePercent(const float& widthPercent, const float& heightPercent)
	{
		setWidthPercent(widthPercent);
		setHeightPercent(heightPercent);
	}

	void Property::setMarginLeft(const float& marginLeft)
	{
		setFloat(PropertyType::MarginLeft, marginLeft);
	}

	void Property::setMarginTop(const float& marginTop)
	{
		setFloat(PropertyType::MarginTop, marginTop);
	}

	void Property::setMarginRight(const float& marginRight)
	{
		setFloat(PropertyType::MarginRight, marginRight);
	}

	void Property::setMarginBottom(const float& marginBottom)
	{
		setFloat(PropertyType::MarginBottom, marginBottom);
	}

	void Property::setMargin(const float& margin)
	{
		setMarginLeft(margin);
		setMarginTop(margin);
		setMarginRight(margin);
		setMarginBottom(margin);
	}

	void Property::setMargin(const float& left, const float& top, const float& right, const float& bottom)
	{
		setMarginLeft(left);
		setMarginTop(top);
		setMarginRight(right);
		setMarginBottom(bottom);
	}

	void Property::setPaddingLeft(const float& paddingLeft)
	{
		setFloat(PropertyType::PaddingLeft, paddingLeft);
	}

	void Property::setPaddingTop(const float& paddingTop)
	{
		setFloat(PropertyType::PaddingTop, paddingTop);
	}

	void Property::setPaddingRight(const float& paddingRight)
	{
		setFloat(PropertyType::PaddingRight, paddingRight);
	}

	void Property::setPaddingBottom(const float& paddingBottom)
	{
		setFloat(PropertyType::PaddingBottom, paddingBottom);
	}

	void Property::setPadding(const float& padding)
	{
		setPaddingLeft(padding);
		setPaddingTop(padding);
		setPaddingRight(padding);
		setPaddingBottom(padding);
	}

	void Property::setPadding(const float& left, const float& top, const float& right, const float& bottom)
	{
		setPaddingLeft(left);
		setPaddingTop(top);
		setPaddingRight(right);
		setPaddingBottom(bottom);
	}

	void Property::setFlexGrow(const float& flexGrow)
	{
		setFloat(PropertyType::FlexGrow, flexGrow);
	}

	void Property::setFlexShrink(const float& flexShrink)
	{
		setFloat(PropertyType::FlexGrow, flexShrink);
	}

	void Property::setWrap(const Wrap& wrap)
	{
		setInt(PropertyType::Wrap, (int&)wrap);
	}

	void Property::setAlign(const Align& align)
	{
		setInt(PropertyType::Align, (int&)align);
	}

	void Property::setJustify(const Justify& justify)
	{
		setInt(PropertyType::Justify, (int&)justify);
	}

	void Property::setFlexDirection(const FlexDirection& flexDirection)
	{
		setInt(PropertyType::FlexDirection, (int&)flexDirection);
	}

	void Property::setBackgroundColor(const Color& color)
	{
		setColor(PropertyType::BackgroundColor, color);
	}

	void Property::setBackgroundHoverColor(const Color& color)
	{
		setColor(PropertyType::BackgroundHoverColor, color);
	}

	void Property::setForegroundColor(const Color& color)
	{
		setColor(PropertyType::ForegroundColor, color);
	}

	void Property::setForegroundHoverColor(const Color& color)
	{
		setColor(PropertyType::BackgroundHoverColor, color);
	}

	void Property::setVisible(bool flag)
	{
		setBool(PropertyType::Visible, flag);
	}

	void Property::setText(const std::wstring& text)
	{
		setText(PropertyType::Text, text);
	}

	void Property::setFontSize(const float& fontSize)
	{
		setFloat(PropertyType::FontSize, fontSize);
	}

	void Property::setFontFamily(const std::wstring& fontFamily)
	{
		setText(PropertyType::FontFamily, fontFamily);
	}

	void Property::setCursor(const HCURSOR cursor)
	{
		setOther(PropertyType::Cursor, (void*)cursor);
	}

	void Property::setCustomData(const std::wstring& key, const std::any& value)
	{
		dataCustom[key] = value;
	}

	const bool Property::hasWidth()
	{
		return dataFloat.contains(PropertyType::Width);
	}

	const bool Property::hasHeight()
	{
		return dataFloat.contains(PropertyType::Height);
	}

	const bool Property::hasWidthPercent()
	{
		return dataFloat.contains(PropertyType::WidthPercent);
	}

	const bool Property::hasHeightPercent()
	{
		return dataFloat.contains(PropertyType::HeightPercent);
	}

	const bool Property::hasMarginLeft()
	{
		return dataFloat.contains(PropertyType::MarginLeft);
	}

	const bool Property::hasMarginTop()
	{
		return dataFloat.contains(PropertyType::MarginTop);
	}

	const bool Property::hasMarginRight()
	{
		return dataFloat.contains(PropertyType::MarginRight);
	}

	const bool Property::hasMarginBottom()
	{
		return dataFloat.contains(PropertyType::MarginBottom);
	}

	const bool Property::hasPaddingLeft()
	{
		return dataFloat.contains(PropertyType::PaddingLeft);
	}

	const bool Property::hasPaddingTop()
	{
		return dataFloat.contains(PropertyType::PaddingTop);
	}

	const bool Property::hasPaddingRight()
	{
		return dataFloat.contains(PropertyType::PaddingRight);
	}

	const bool Property::hasPaddingBottom()
	{
		return dataFloat.contains(PropertyType::PaddingBottom);
	}

	const bool Property::hasFlexGrow()
	{
		return dataFloat.contains(PropertyType::FlexGrow);
	}

	const bool Property::hasFlexShrink()
	{
		return dataFloat.contains(PropertyType::FlexShrink);
	}

	const bool Property::hasColorBackground()
	{
		return dataColor.contains(PropertyType::BackgroundColor);
	}

	const bool Property::hasColorBackgroundHover()
	{
		return dataColor.contains(PropertyType::BackgroundHoverColor);
	}

	const bool Property::hasColorForeground()
	{
		return dataColor.contains(PropertyType::ForegroundColor);
	}

	const bool Property::hasColorForegroundHover()
	{
		return dataColor.contains(PropertyType::ForegroundHoverColor);
	}

	const bool Property::hasVisible()
	{
		return dataBool.contains(PropertyType::Visible);
	}

	const bool Property::hasText()
	{
		return dataText.contains(PropertyType::Text);
	}

	const bool Property::hasFontSize()
	{
		return dataFloat.contains(PropertyType::FontSize);
	}

	const bool Property::hasFontFamily()
	{
		return dataText.contains(PropertyType::FontFamily);
	}

	const bool Property::hasFontWeight()
	{
		return dataFloat.contains(PropertyType::FontWeight);
	}

	const bool Property::hasCursor()
	{
		return dataOther.contains(PropertyType::Cursor);
	}

	const float& Property::getWidth()
	{
		return dataFloat[PropertyType::Width];
	}
	const float& Property::getHeight()
	{
		return dataFloat[PropertyType::Height];
	}
	const float& Property::getWidthPercent()
	{
		return dataFloat[PropertyType::WidthPercent];
	}
	const float& Property::getHeightPercent()
	{
		return dataFloat[PropertyType::HeightPercent];
	}
	const float& Property::getMarginLeft()
	{
		return dataFloat[PropertyType::MarginLeft];
	}
	const float& Property::getMarginTop()
	{
		return dataFloat[PropertyType::MarginTop];
	}
	const float& Property::getMarginRight()
	{
		return dataFloat[PropertyType::MarginRight];
	}
	const float& Property::getMarginBottom()
	{
		return dataFloat[PropertyType::MarginBottom];
	}
	const float& Property::getPaddingLeft()
	{
		return dataFloat[PropertyType::PaddingLeft];
	}
	const float& Property::getPaddingTop()
	{
		return dataFloat[PropertyType::PaddingTop];
	}
	const float& Property::getPaddingRight()
	{
		return dataFloat[PropertyType::PaddingRight];
	}
	const float& Property::getPaddingBottom()
	{
		return dataFloat[PropertyType::PaddingBottom];
	}
	const float& Property::getFlexGrow()
	{
		return dataFloat[PropertyType::FlexGrow];
	}
	const float& Property::getFlexShrink()
	{
		return dataFloat[PropertyType::FlexShrink];
	}
	const float& Property::getFontSize()
	{
		return dataFloat[PropertyType::FontSize];
	}

	const Wrap& Property::getWrap()
	{
		return (Wrap&)dataInt[PropertyType::Wrap];
	}
	const Align& Property::getAlign()
	{
		return (Align&)dataInt[PropertyType::Align];
	}
	const Justify& Property::getJustify()
	{
		return (Justify&)dataInt[PropertyType::Justify];
	}
	const FlexDirection& Property::getFlexDirection()
	{
		return (FlexDirection&)dataInt[PropertyType::FlexDirection];
	}
	const Color& Property::getColorBackground()
	{
		return dataColor[PropertyType::BackgroundColor];
	}
	const Color& Property::getColorBackgroundHover()
	{
		return dataColor[PropertyType::BackgroundHoverColor];
	}
	const Color& Property::getColorForeground()
	{
		return dataColor[PropertyType::ForegroundColor];
	}
	const Color& Property::getColorForegroundHover()
	{
		return dataColor[PropertyType::ForegroundHoverColor];
	}

	const bool& Property::getVisible()
	{
		return dataBool[PropertyType::Visible];
	}

	const std::wstring& Property::getText()
	{
		return dataText[PropertyType::Text];
	}

	const std::wstring& Property::getFontFamily()
	{
		return dataText[PropertyType::FontFamily];
	}

	const HCURSOR Property::getCursor()
	{
		return (HCURSOR)(dataOther[PropertyType::Cursor]);
	}

	const std::any& Property::getCustomData(const std::wstring& key)
	{
		return dataCustom[key];
	}
	
	void Property::setFloat(const PropertyType& type, const float& value)
	{
		if (dataFloat.contains(type)) {
			if (dataFloat[type] != value) {
				dataFloat[type] = value;
				changeElementProperty(type, &dataFloat[type]);
			}
		}
		else {
			dataFloat.insert({ type,value });
			changeElementProperty(type, &dataFloat[type]);
		}
	}

	void Property::setInt(const PropertyType& type, const int& value)
	{
		if (dataInt.contains(type)) {
			if (dataInt[type] != value) {
				dataInt[type] = value;
				changeElementProperty(type, &dataInt[type]);
			}
		}
		else {
			dataInt.insert({ type,value });
			changeElementProperty(type, &dataInt[type]);
		}
	}

	void Property::setBool(const PropertyType& type, const bool& value)
	{
		if (dataBool.contains(type)) {
			if (dataBool[type] != value) {
				dataBool[type] = value;
				changeElementProperty(type, &dataBool[type]);
			}
		}
		else {
			dataBool.insert({ type,value });
			changeElementProperty(type, &dataBool[type]);
		}
	}

	void Property::setText(const PropertyType& type, const std::wstring& value)
	{
		if (dataText.contains(type)) {
			if (dataText[type] != value) {
				dataText[type] = value;
				changeElementProperty(type, &dataText[type]);
			}
		}
		else {
			dataText.insert({ type,value });
			changeElementProperty(type, &dataText[type]);
		}
	}

	void Property::setColor(const PropertyType& type, const Color& value)
	{
		if (dataColor.contains(type)) {
			if (dataColor[type].equals(value)) {
				dataColor[type] = value;
				changeElementProperty(type, &dataColor[type]);
			}
		}
		else {
			dataColor.insert({ type,value });
			changeElementProperty(type, &dataColor[type]);
		}
	}

	void Property::setOther(const Ling::PropertyType& type,void* value)
	{
		if (dataOther.contains(type)) {
			if (dataOther[type] != value) {
				dataOther[type] = value;
				changeElementProperty(type, dataOther[type]);
			}
		}
		else {
			dataOther.insert({ type,value });
			changeElementProperty(type, dataOther[type]);
		}
	}

	void Property::changeElementProperty(const Ling::PropertyType& type, const void* value)
	{
		for (auto ele:elements)
		{
			ele->propertyChanged(type, value);
		}
	}

}
