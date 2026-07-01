#pragma once
#include <winrt/Windows.Foundation.h>
#include <yoga/Yoga.h>
#include "Color.h"
#include "Wrap.h"
#include "Align.h"
#include "Justify.h"
#include "FlexDirection.h"
namespace Ling {
	using namespace winrt::Windows::Foundation;
	class StyleBase
	{
		using EventHandler = winrt::delegate<void(StyleBase*)>;
	public:
		StyleBase();
		virtual ~StyleBase();
		auto onChange(EventHandler const& handler);
		void offChange(winrt::event_token const& token);

		// get 方法
		float getLeft();
		float getTop();
		float getWidth();
		float getHeight();
		float getMarginLeft();
		float getMarginTop();
		float getMarginRight();
		float getMarginBottom();
		float getPaddingLeft();
		float getPaddingTop();
		float getPaddingRight();
		float getPaddingBottom();
		// set 方法
		void setBackgroundColor(const Color& color);
		void setHoverBackgroundColor(const Color& color);
		void setCursor(LPCWSTR cursor);
		void setWidth(const float& w);
		void setHeight(const float& h);
		void setSize(const float& w, const float& h);
		void setWidthPercent(const float& percent);
		void setHeightPercent(const float& percent);
		void setSizePercent(const float& w, const float& h);
		void setFlexGrow(const float& val);
		void setFlexShrink(const float& val);
		void setMargin(const float& val);
		void setMargin(const float& left, const float& top, const float& right, const float& bottom);
		void setMarginLeft(const float& val);
		void setMarginTop(const float& val);
		void setMarginRight(const float& val);
		void setMarginBottom(const float& val);
		void setPadding(const float& val);
		void setPadding(const float& left, const float& top, const float& right, const float& bottom);
		void setPaddingLeft(const float& val);
		void setPaddingTop(const float& val);
		void setPaddingRight(const float& val);
		void setPaddingBottom(const float& val);
		void setFlexWrap(const Wrap& val);
		void setAlignItems(const Align& val);
		void setJustifyContent(const Justify& val);
		void setFlexDirection(const FlexDirection& flexDirection);
	protected:
		void notifyChange();
	private:
		winrt::event<EventHandler> events;
		float left, top, width, height, marginLeft, marginTop, marginRight, marginBottom, paddingLeft, paddingTop, paddingRight, paddingBottom;
	};
}

