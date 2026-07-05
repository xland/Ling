#pragma once
#include "Color.h"
#include "Wrap.h"
#include "Align.h"
#include "Justify.h"
#include "FlexDirection.h"
namespace Ling {
	class Element;
	class ElementStyle
	{
		friend class Element;
	public:
		ElementStyle();
		virtual ~ElementStyle();

		void setVisible(bool flag);
		void setCursor(const HCURSOR cursor);
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

		bool getVisible();
		HCURSOR getCursor();
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
		Color getColorBackground();
		Color getBackgroundHoverColor();
	protected:
		std::vector<Element*> elements;
	private:
	private:
		bool visible{ true };
		HCURSOR cursor{ nullptr };
		float width{ NAN };
		float height{ NAN };
		float widthPercent{ NAN };
		float heightPercent{ NAN };
		float marginLeft{ NAN };
		float marginTop{ NAN };
		float marginRight{ NAN };
		float marginBottom{ NAN };
		float paddingLeft{ NAN };
		float paddingTop{ NAN };
		float paddingRight{ NAN };
		float paddingBottom{ NAN };
		float flexGrow{ NAN };
		float flexShrink{ NAN };
		Wrap wrap{ Wrap::NoWrap };
		Align align{ Align::Auto };
		Justify justify{ Justify::Auto };
		FlexDirection flexDirection{ FlexDirection::Row };
		Color backgroundColor{ 0x00000000 };
		Color backgroundHoverColor{ 0x00000000 };
	};
}
