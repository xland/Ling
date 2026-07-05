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
	class Element;
	class Property
	{
		friend class Element;
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

		void setBackgroundColor(const Color& color);
		void setBackgroundHoverColor(const Color& color);
		void setForegroundColor(const Color& color);
		void setForegroundHoverColor(const Color& color);

		void setVisible(bool flag);
		void setText(const std::wstring& text);
		void setFontSize(const float& fontSize);
		void setFontFamily(const std::wstring& fontFamily);
		void setCursor(const HCURSOR cursor);

		void setCustomData(const std::wstring& key,const std::any& value);
		
		const bool hasWidth();
		const bool hasHeight();
		const bool hasWidthPercent();
		const bool hasHeightPercent();
		const bool hasMarginLeft();
		const bool hasMarginTop();
		const bool hasMarginRight();
		const bool hasMarginBottom();
		const bool hasPaddingLeft();
		const bool hasPaddingTop();
		const bool hasPaddingRight();
		const bool hasPaddingBottom();
		const bool hasFlexGrow();
		const bool hasFlexShrink();
		const bool hasColorBackground();
		const bool hasColorBackgroundHover();
		const bool hasColorForeground();
		const bool hasColorForegroundHover();
		const bool hasVisible();
		const bool hasText();
		const bool hasFontSize();
		const bool hasFontFamily();
		const bool hasFontWeight();
		const bool hasCursor();

		const float& getWidth();
		const float& getHeight();
		const float& getWidthPercent();
		const float& getHeightPercent();
		const float& getMarginLeft();
		const float& getMarginTop();
		const float& getMarginRight();
		const float& getMarginBottom();
		const float& getPaddingLeft();
		const float& getPaddingTop();
		const float& getPaddingRight();
		const float& getPaddingBottom();
		const float& getFlexGrow();
		const float& getFlexShrink();
		const float& getFontSize();
		const Color& getColorBackground();
		const Color& getColorBackgroundHover();
		const Color& getColorForeground();
		const Color& getColorForegroundHover();
		const bool& getVisible();
		const std::wstring& getText();
		const std::wstring& getFontFamily();
		const HCURSOR getCursor();
		const std::any& getCustomData(const std::wstring& key);

		const Wrap& getWrap();
		const Align& getAlign();
		const Justify& getJustify();
		const FlexDirection& getFlexDirection();
	private:
		void setFloat(const Ling::PropertyType& type, const float& value);
		void setInt(const Ling::PropertyType& type, const int& value);
		void setBool(const Ling::PropertyType& type, const bool& value);
		void setText(const Ling::PropertyType& type, const std::wstring& value);
		void setColor(const Ling::PropertyType& type, const Color& value);
		void setOther(const Ling::PropertyType& type, void* value);
		void changeElementProperty(const Ling::PropertyType& type, const void* value);
	private:
		std::unordered_map<Ling::PropertyType, float> dataFloat;
		std::unordered_map<Ling::PropertyType, int> dataInt;
		std::unordered_map<Ling::PropertyType, bool> dataBool;
		std::unordered_map<Ling::PropertyType, std::wstring> dataText;
		std::unordered_map<Ling::PropertyType, Color> dataColor;
		std::unordered_map<Ling::PropertyType, void*> dataOther;
		std::unordered_map<std::wstring, std::any> dataCustom;
		std::vector<Element*> elements;
	};
}

