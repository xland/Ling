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
		if (setFloat(PropertyType::Width, width)) {
			for (auto& ele : elements)
			{
				ele->setWidth(width);
			}
		}
	}

	void Property::setHeight(const float& height)
	{
		if (setFloat(PropertyType::Height, height)) {
			for (auto& ele : elements)
			{
				ele->setHeight(height);
			}
		}
	}

	void Property::setSize(const float& width, const float& height)
	{
		setWidth(width);
		setHeight(height);
	}

	void Property::setWidthPercent(const float& widthPercent)
	{
		if (setFloat(PropertyType::WidthPercent, widthPercent)) {
			for (auto& ele : elements)
			{
				ele->setWidthPercent(widthPercent);
			}
		}
	}

	void Property::setHeightPercent(const float& heightPercent)
	{
		if (setFloat(PropertyType::WidthPercent, heightPercent)) {
			for (auto& ele : elements)
			{
				ele->setHeightPercent(heightPercent);
			}
		}
	}

	void Property::setSizePercent(const float& widthPercent, const float& heightPercent)
	{
		setWidthPercent(widthPercent);
		setHeightPercent(heightPercent);
	}

	void Property::setMarginLeft(const float& marginLeft)
	{
		if (setFloat(PropertyType::MarginLeft, marginLeft)) {
			for (auto& ele : elements)
			{
				ele->setMarginLeft(marginLeft);
			}
		}
	}

	void Property::setMarginTop(const float& marginTop)
	{
		if (setFloat(PropertyType::MarginTop, marginTop)) {
			for (auto& ele : elements)
			{
				ele->setMarginTop(marginTop);
			}
		}
	}

	void Property::setMarginRight(const float& marginRight)
	{
		if (setFloat(PropertyType::MarginRight, marginRight)) {
			for (auto& ele : elements)
			{
				ele->setMarginRight(marginRight);
			}
		}
	}

	void Property::setMarginBottom(const float& marginBottom)
	{
		if (setFloat(PropertyType::MarginBottom, marginBottom)) {
			for (auto& ele : elements)
			{
				ele->setMarginBottom(marginBottom);
			}
		}
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
		if (setFloat(PropertyType::PaddingLeft, paddingLeft)) {
			for (auto& ele : elements)
			{
				ele->setPaddingLeft(paddingLeft);
			}
		}
	}

	void Property::setPaddingTop(const float& paddingTop)
	{
		if (setFloat(PropertyType::PaddingTop, paddingTop)) {
			for (auto& ele : elements)
			{
				ele->setPaddingTop(paddingTop);
			}
		}
	}

	void Property::setPaddingRight(const float& paddingRight)
	{
		if (setFloat(PropertyType::PaddingRight, paddingRight)) {
			for (auto& ele : elements)
			{
				ele->setPaddingRight(paddingRight);
			}
		}
	}

	void Property::setPaddingBottom(const float& paddingBottom)
	{
		if (setFloat(PropertyType::PaddingBottom, paddingBottom)) {
			for (auto& ele : elements)
			{
				ele->setPaddingBottom(paddingBottom);
			}
		}
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
		if (setFloat(PropertyType::FlexGrow, flexGrow)) {
			for (auto& ele : elements)
			{
				ele->setFlexGrow(flexGrow);
			}
		}
	}

	void Property::setFlexShrink(const float& flexShrink)
	{
		if (setFloat(PropertyType::FlexGrow, flexShrink)) {
			for (auto& ele : elements)
			{
				ele->setFlexShrink(flexShrink);
			}
		}
	}

	void Property::setWrap(const Wrap& wrap)
	{
		if (setInt(PropertyType::Wrap, (int&)wrap)) {
			for (auto& ele : elements)
			{
				ele->setWrap(wrap);
			}
		}
	}

	void Property::setAlign(const Align& align)
	{
		if (setInt(PropertyType::Align, (int&)align)) {
			for (auto& ele : elements)
			{
				ele->setAlign(align);
			}
		}
	}

	void Property::setJustify(const Justify& justify)
	{
		if (setInt(PropertyType::Justify, (int&)justify)) {
			for (auto& ele : elements)
			{
				ele->setJustify(justify);
			}
		}
	}

	void Property::setFlexDirection(const FlexDirection& flexDirection)
	{
		if (setInt(PropertyType::FlexDirection, (int&)flexDirection)) {
			for (auto& ele : elements)
			{
				ele->setFlexDirection(flexDirection);
			}
		}
	}

	void Property::setColorBackground(const Color& color)
	{
		if (setColor(PropertyType::ColorBackground, color)) {
			for (auto& ele : elements)
			{
				ele->setColorBackground(color);
			}
		}
	}

	void Property::setColorBackgroundHover(const Color& color)
	{
		if (setColor(PropertyType::ColorBackgroundHover, color)) {
			for (auto& ele : elements)
			{
				ele->setColorBackgroundHover(color);
			}
		}
	}

	void Property::setColorForeground(const Color& color)
	{
		if (setColor(PropertyType::ColorForeground, color)) {
			for (auto& ele : elements)
			{
				//ele.setWidth(val);
			}
		}
	}

	void Property::setColorForegroundHover(const Color& color)
	{
		if (setColor(PropertyType::ColorBackgroundHover, color)) {
			for (auto& ele : elements)
			{
				//ele.setWidth(val);
			}
		}
	}

	void Property::setVisible(bool flag)
	{
		if (setBool(PropertyType::Visible, flag)) {
			for (auto& ele : elements)
			{
				//ele.setWidth(val);
			}
		}
	}

	void Property::setText(const std::wstring& text)
	{
		if (setText(PropertyType::Text, text)) {
			for (auto& ele : elements)
			{
				//ele.setWidth(val);
			}
		}
	}

	void Property::setCustomData(const std::wstring& key, const std::any& value)
	{
		dataCustom[key] = value;
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
	const Wrap& Property::getWrap()
	{
		return (Wrap)dataInt[PropertyType::Wrap];
	}
	const Align& Property::getAlign()
	{
		return (Align)dataInt[PropertyType::Align];
	}
	const Justify& Property::getJustify()
	{
		return (Justify)dataInt[PropertyType::Justify];
	}
	const FlexDirection& Property::getFlexDirection()
	{
		return (FlexDirection)dataInt[PropertyType::FlexDirection];
	}
	const Color& Property::getColorBackground()
	{
		return dataColor[PropertyType::ColorBackground];
	}
	const Color& Property::getColorBackgroundHover()
	{
		return dataColor[PropertyType::ColorBackgroundHover];
	}
	const Color& Property::getColorForeground()
	{
		return dataColor[PropertyType::ColorForeground];
	}
	const Color& Property::getColorForegroundHover()
	{
		return dataColor[PropertyType::ColorForegroundHover];
	}

	const bool& Property::getVisible()
	{
		return dataBool[PropertyType::Visible];
	}

	const std::wstring& Property::getText()
	{
		return dataText[PropertyType::Text];
	}

	const std::any& Property::getCustomData(const std::wstring& key)
	{
		return dataCustom[key];
	}
	
	bool Property::setFloat(const PropertyType& type, const float& value)
	{
		if (dataFloat.contains(type)) {
			if (dataFloat[type] != value) {
				dataFloat[type] = value;
				return true;
			}
		}
		else {
			dataFloat.insert({ type,value });
			return true;
		}
		return false;
	}

	bool Property::setInt(const PropertyType& type, const int& value)
	{
		if (dataInt.contains(type)) {
			if (dataInt[type] != value) {
				dataInt[type] = value;
				return true;
			}
		}
		else {
			dataInt.insert({ type,value });
			return true;
		}
		return false;
	}

	bool Property::setBool(const PropertyType& type, const bool& value)
	{
		if (dataBool.contains(type)) {
			if (dataBool[type] != value) {
				dataBool[type] = value;
				return true;
			}
		}
		else {
			dataBool.insert({ type,value });
			return true;
		}
		return false;
	}

	bool Property::setText(const PropertyType& type, const std::wstring& value)
	{
		if (dataText.contains(type)) {
			if (dataText[type] != value) {
				dataText[type] = value;
				return true;
			}
		}
		else {
			dataText.insert({ type,value });
			return true;
		}
		return false;
	}

	bool Property::setColor(const PropertyType& type, const Color& value)
	{
		if (dataColor.contains(type)) {
			if (dataColor[type].equals(value)) {
				dataColor[type] = value;
				return true;
			}
		}
		else {
			dataColor.insert({ type,value });
			return true;
		}
		return false;
	}

}
