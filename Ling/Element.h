#pragma once
#include "Header.h"
#include <yoga/Yoga.h>
#include "Color.h"
#include "Wrap.h"
#include "Align.h"
#include "Justify.h"
#include "FlexDirection.h"
#include "MouseButton.h"
#include "MouseEvent.h"

namespace Ling {
	class WindowBase;

	class Element
	{
	public:
		Element(WindowBase* win);
		virtual ~Element();

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

		// 事件方法
		void mouseEnter(const MouseEvent& event);
		void mouseLeave(const MouseEvent& event);
		void mouseMove(const MouseEvent& event);
		void mouseDown(const MouseEvent& event);
		void mouseUp(const MouseEvent& event);
		size_t onMouseEnter(std::function<void(const MouseEvent&)> callback);
		size_t onMouseLeave(std::function<void(const MouseEvent&)> callback);
		size_t onMouseMove(std::function<void(const MouseEvent&)> callback);
		size_t onMouseDown(std::function<void(const MouseEvent&)> callback);
		size_t onMouseUp(std::function<void(const MouseEvent&)> callback);
		void offMouseEnter(const size_t& callbackId);
		void offMouseLeave(const size_t& callbackId);
		void offMouseMove(const size_t& callbackId);
		void offMouseDown(const size_t& callbackId);
		void offMouseUp(const size_t& callbackId);

		// set 方法（非链式，返回 void）
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

	protected:
		virtual void initProperty();

	public:
		float xAbs{ 0.f }, yAbs{ 0.f };
		WindowBase* win{ nullptr };
		Composition::SpriteVisual visual{ nullptr };
		YGNodeRef node{ nullptr };
		Element* parent{ nullptr };

	protected:
		std::vector<Element*> children;
		float x{ 0.f }, y{ 0.f }, w{ 0.f }, h{ 0.f };
		Color backgroundColor{ 0X00000000 };
		Color hoverBackgroundColor{ 0X00000000 };
		HCURSOR cursor{ nullptr };

	private:
		size_t mouseMoveCBId{ 0 };
		std::unordered_map<size_t, std::function<void(const MouseEvent&)>> mouseMoveCBs;
		size_t mouseDownCBId{ 0 };
		std::unordered_map<size_t, std::function<void(const MouseEvent&)>> mouseDownCBs;
		size_t mouseUpCBId{ 0 };
		std::unordered_map<size_t, std::function<void(const MouseEvent&)>> mouseUpCBs;
		size_t mouseEnterCBId{ 0 };
		std::unordered_map<size_t, std::function<void(const MouseEvent&)>> mouseEnterCBs;
		size_t mouseLeaveCBId{ 0 };
		std::unordered_map<size_t, std::function<void(const MouseEvent&)>> mouseLeaveCBs;
	};
}