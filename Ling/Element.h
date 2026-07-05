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

namespace Ling {
	using namespace winrt::Windows::UI;
	class WindowBase;
	class ElementStyle;
	class Element
	{
		friend class ElementStyle;
	public:
		Element(WindowBase* win);
		virtual ~Element();
		std::shared_ptr<ElementStyle> createStyle();
		virtual void shareStyle(const std::shared_ptr<ElementStyle>& style);
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
	protected:
		std::vector<Element*> children;
		std::shared_ptr<ElementStyle> style;
		float x{ 0.f }, y{ 0.f }, w{ 0.f }, h{ 0.f };
	private:
		virtual void setWidth(const float& width);
		virtual void setHeight(const float& height);
		virtual void setWidthPercent(const float& widthPercent);
		virtual void setHeightPercent(const float& heightPercent);
		virtual void setMarginLeft(const float& marginLeft);
		virtual void setMarginTop(const float& marginTop);
		virtual void setMarginRight(const float& marginRight);
		virtual void setMarginBottom(const float& marginBottom);
		virtual void setPaddingLeft(const float& paddingLeft);
		virtual void setPaddingTop(const float& paddingTop);
		virtual void setPaddingRight(const float& paddingRight);
		virtual void setPaddingBottom(const float& paddingBottom);
		virtual void setFlexGrow(const float& flexGrow);
		virtual void setFlexShrink(const float& flexShrink);
		virtual void setWrap(const Wrap& wrap);
		virtual void setAlign(const Align& align);
		virtual void setJustify(const Justify& justify);
		virtual void setFlexDirection(const FlexDirection& flexDirection);
		virtual void setBackgroundColor(const Color& color);
		virtual void setBackgroundHoverColor(const Color& color);
		virtual void setForegroundColor(const Color& color);
		virtual void setForegroundHoverColor(const Color& color);
		virtual void setVisible(const bool& flag);
		virtual void setText(const std::wstring& text);
		virtual void setCursor(const HCURSOR cursor);

	private:
		bool isMouseEnter{ false };
		winrt::event<winrt::delegate<void(const MouseEvent&)>> eventMouseMove;
		winrt::event<winrt::delegate<void(const MouseEvent&)>> eventMouseDown;
		winrt::event<winrt::delegate<void(const MouseEvent&)>> eventMouseUp;
		winrt::event<winrt::delegate<void(const MouseEvent&)>> eventMouseEnter;
		winrt::event<winrt::delegate<void(const MouseEvent&)>> eventMouseLeave;
	};
}


