#pragma once
#include <winrt/Windows.Foundation.h>
#include <yoga/Yoga.h>
#include <variant>
#include <any>
#include "Color.h"
#include "Wrap.h"
#include "Align.h"
#include "Justify.h"
#include "FlexDirection.h"
#include "PropertyType.h"
namespace Ling {
	class IElement;
	class Property
	{
	public:
		Property();
		virtual ~Property();	
		void setWidth(const float& width);
		void setHeight(const float& height);
		void setSize(const float& width, const float& height);

		void setWidthPercent(const float& widthPercent);
		void setHeightPercent(const float& heightPercent);
		void setSizePercent(const float& widthPercent, const float& heightPercent);

		void setMarginLeft(const float& marginLeft);
		void setMarginTop(const float& marginTop);
		void setMarginRight(const float& marginRight);
		void setMarginBottom(const float& marginBottom);
		void setMargin(const float& margin);
		void setMargin(const float& left, const float& top, const float& right, const float& bottom);

		void setPaddingLeft(const float& paddingLeft);
		void setPaddingTop(const float& paddingTop);
		void setPaddingRight(const float& paddingRight);
		void setPaddingBottom(const float& paddingBottom);
		void setPadding(const float& padding);
		void setPadding(const float& left, const float& top, const float& right, const float& bottom);

		void setFlexGrow(const float& flexGrow);
		void setFlexShrink(const float& flexShrink);

		void setWrap(const Wrap& wrap);
		void setAlign(const Align& align);
		void setJustify(const Justify& justify);
		void setFlexDirection(const FlexDirection& flexDirection);
		
		float getWidth();
		float getHeight();
		float getWidthPercent();
		float getHeightPercent();
		float getMarginLeft();
		float getMarginTop();
		float getMarginRight();
		float getMarginBottom();
		float getPaddingLeft();
		float getPaddingTop();
		float getPaddingRight();
		float getPaddingBottom();
		float getFlexGrow();
		float getFlexShrink();

		Wrap getWrap();
		Align getAlign();
		Justify getJustify();
		FlexDirection getFlexDirection();
	private:
		bool setFloat(const PropertyType& type, const float& value);
		bool setInt(const PropertyType& type, const int& value);
	private:
		std::unordered_map<PropertyType, float> dataFloat;
		std::unordered_map<PropertyType, int> dataInt;
		std::unordered_map<PropertyType, bool> dataBool;
		std::unordered_map<PropertyType, std::wstring> dataStr;
		std::unordered_map<std::wstring, std::any> dataCustom;
		std::vector<IElement*> elements;
	};
}

