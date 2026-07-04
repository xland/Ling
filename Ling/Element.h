#pragma once
#include <functional>
#include <vector>
#include <unordered_map>
#include <Windows.h>
#include <yoga/Yoga.h>
#include <variant>
#include <winrt/Windows.Foundation.h>
#include <winrt/Windows.UI.Composition.h>
#include <any>
#include "Color.h"
#include "Wrap.h"
#include "Align.h"
#include "Justify.h"
#include "FlexDirection.h"
#include "MouseButton.h"
#include "MouseEvent.h"
#include "PropertyType.h"

namespace Ling {
	using namespace winrt::Windows::UI;
	class WindowBase;
	class Property;
	class Element
	{
		friend class Property;
	public:
		Element(WindowBase* win);
		virtual ~Element();
		void setProperty(const std::shared_ptr<Property>& property);
		// 多态方法
		bool isAncestor(const Element* target);
		Element* findAncestor(Element* other);
		void removeSelf();
		void removeChild(Element* ele);
		void insertChild(const int& index, Element* ele);
		void addChild(Element* ele);
		virtual void layout();
		bool setCursor();
		Element* hitTest(const int& x, const int& y);
		bool containPosition(const int& x, const int& y);
		// 事件方法
		void mouseEnter(const MouseEvent& event);
		void mouseLeave(const MouseEvent& event);
		void mouseMove(const MouseEvent& event);
		void mouseDown(const MouseEvent& event);
		void mouseUp(const MouseEvent& event);
		winrt::event_token onMouseEnter(winrt::delegate<void(const MouseEvent&)> const& handler);
		winrt::event_token onMouseLeave(winrt::delegate<void(const MouseEvent&)> const& handler);
		winrt::event_token onMouseMove(winrt::delegate<void(const MouseEvent&)> const& handler);
		winrt::event_token onMouseDown(winrt::delegate<void(const MouseEvent&)> const& handler);
		winrt::event_token onMouseUp(winrt::delegate<void(const MouseEvent&)> const& handler);
		void offMouseEnter(const winrt::event_token& callbackId);
		void offMouseLeave(const winrt::event_token& callbackId);
		void offMouseMove(const winrt::event_token& callbackId);
		void offMouseDown(const winrt::event_token& callbackId);
		void offMouseUp(const winrt::event_token& callbackId);
	public:
		float xAbs{ 0.f }, yAbs{ 0.f };
		WindowBase* win{ nullptr };
		Composition::SpriteVisual visual{ nullptr };
		YGNodeRef node{ nullptr };
		Element* parent{ nullptr };
		std::wstring id;
	protected:
		virtual void initProperty() {};
	protected:
		std::vector<Element*> children;
		float x{ 0.f }, y{ 0.f }, w{ 0.f }, h{ 0.f };
	private:
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

		void setColorBackground(const Color& color);
		void setColorBackgroundHover(const Color& color);
		void setColorForeground(const Color& color);
		void setColorForegroundHover(const Color& color);
		
		virtual void propertyFloatChanged(const Ling::PropertyType& key, const float& val);
		virtual void propertyIntChanged(const Ling::PropertyType& key, const int& val);
		virtual void propertyColorChanged(const Ling::PropertyType& key, const Color& val);
		virtual void propertyBoolChanged(const Ling::PropertyType& key, const bool& val);
		virtual void propertyTextChanged(const Ling::PropertyType& key, const std::wstring& val);
		virtual void propertyOtherChanged(const Ling::PropertyType& key, const std::any& val);

	private:
		bool isMouseEnter{ false };
		winrt::event<winrt::delegate<void(const MouseEvent&)>> eventMouseMove;
		winrt::event<winrt::delegate<void(const MouseEvent&)>> eventMouseDown;
		winrt::event<winrt::delegate<void(const MouseEvent&)>> eventMouseUp;
		winrt::event<winrt::delegate<void(const MouseEvent&)>> eventMouseEnter;
		winrt::event<winrt::delegate<void(const MouseEvent&)>> eventMouseLeave;
		std::shared_ptr<Property> property;
	};
}


