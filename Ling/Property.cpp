#include "pch.h"
#include "Property.h"
#include "IElement.h"
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
				//ele.setWidth(val);
			}
		}
	}

	void Property::setHeight(const float& height)
	{
		if (setFloat(PropertyType::Height, height)) {
			for (auto& ele : elements)
			{
				//ele.setHeight(val);
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
				//ele.WidthPercent(val);
			}
		}
	}

	void Property::setHeightPercent(const float& heightPercent)
	{
		if (setFloat(PropertyType::WidthPercent, heightPercent)) {
			for (auto& ele : elements)
			{
				//ele.WidthPercent(val);
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
				//ele.setWidth(val);
			}
		}
	}

	void Property::setMarginTop(const float& marginTop)
	{
		if (setFloat(PropertyType::MarginTop, marginTop)) {
			for (auto& ele : elements)
			{
				//ele.setWidth(val);
			}
		}
	}

	void Property::setMarginRight(const float& marginRight)
	{
		if (setFloat(PropertyType::MarginRight, marginRight)) {
			for (auto& ele : elements)
			{
				//ele.setWidth(val);
			}
		}
	}

	void Property::setMarginBottom(const float& marginBottom)
	{
		if (setFloat(PropertyType::MarginBottom, marginBottom)) {
			for (auto& ele : elements)
			{
				//ele.setWidth(val);
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
				//ele.setWidth(val);
			}
		}
	}

	void Property::setPaddingTop(const float& paddingTop)
	{
		if (setFloat(PropertyType::PaddingTop, paddingTop)) {
			for (auto& ele : elements)
			{
				//ele.setWidth(val);
			}
		}
	}

	void Property::setPaddingRight(const float& paddingRight)
	{
		if (setFloat(PropertyType::PaddingRight, paddingRight)) {
			for (auto& ele : elements)
			{
				//ele.setWidth(val);
			}
		}
	}

	void Property::setPaddingBottom(const float& paddingBottom)
	{
		if (setFloat(PropertyType::PaddingBottom, paddingBottom)) {
			for (auto& ele : elements)
			{
				//ele.setWidth(val);
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
				//ele.setWidth(val);
			}
		}
	}

	void Property::setFlexShrink(const float& flexShrink)
	{
		if (setFloat(PropertyType::FlexGrow, flexShrink)) {
			for (auto& ele : elements)
			{
				//ele.setWidth(val);
			}
		}
	}

	void Property::setWrap(const Wrap& wrap)
	{
		if (setInt(PropertyType::Wrap, (int&)wrap)) {
			for (auto& ele : elements)
			{
				//ele.setWidth(val);
			}
		}
	}

	void Property::setAlign(const Align& align)
	{
		if (setInt(PropertyType::Align, (int&)align)) {
			for (auto& ele : elements)
			{
				//ele.setWidth(val);
			}
		}
	}

	void Property::setJustify(const Justify& justify)
	{
		if (setInt(PropertyType::Justify, (int&)justify)) {
			for (auto& ele : elements)
			{
				//ele.setWidth(val);
			}
		}
	}

	void Property::setFlexDirection(const FlexDirection& flexDirection)
	{
		if (setInt(PropertyType::FlexDirection, (int&)flexDirection)) {
			for (auto& ele : elements)
			{
				//ele.setWidth(val);
			}
		}
	}

	float Property::getWidth()
	{
		return dataFloat[PropertyType::Width];
	}
	float Property::getHeight()
	{
		return dataFloat[PropertyType::Height];
	}
	float Property::getWidthPercent()
	{
		return dataFloat[PropertyType::WidthPercent];
	}
	float Property::getHeightPercent()
	{
		return dataFloat[PropertyType::HeightPercent];
	}
	float Property::getMarginLeft()
	{
		return dataFloat[PropertyType::MarginLeft];
	}
	float Property::getMarginTop()
	{
		return dataFloat[PropertyType::MarginTop];
	}
	float Property::getMarginRight()
	{
		return dataFloat[PropertyType::MarginRight];
	}
	float Property::getMarginBottom()
	{
		return dataFloat[PropertyType::MarginBottom];
	}
	float Property::getPaddingLeft()
	{
		return dataFloat[PropertyType::PaddingLeft];
	}
	float Property::getPaddingTop()
	{
		return dataFloat[PropertyType::PaddingTop];
	}
	float Property::getPaddingRight()
	{
		return dataFloat[PropertyType::PaddingRight];
	}
	float Property::getPaddingBottom()
	{
		return dataFloat[PropertyType::PaddingBottom];
	}
	float Property::getFlexGrow()
	{
		return dataFloat[PropertyType::FlexGrow];
	}
	float Property::getFlexShrink()
	{
		return dataFloat[PropertyType::FlexShrink];
	}
	Wrap Property::getWrap()
	{
		return (Wrap)dataInt[PropertyType::Wrap];
	}
	Align Property::getAlign()
	{
		return (Align)dataInt[PropertyType::Align];
	}
	Justify Property::getJustify()
	{
		return (Justify)dataInt[PropertyType::Justify];
	}
	FlexDirection Property::getFlexDirection()
	{
		return (FlexDirection)dataInt[PropertyType::FlexDirection];
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

}
