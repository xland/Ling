#pragma once
#include <functional>
#include <vector>
#include <unordered_map>
#include <Windows.h>
#include <yoga/Yoga.h>
#include <variant>
#include <winrt/Windows.UI.Composition.h>
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
		bool containPosition(const int& x, const int& y);
		void changeProperty(const std::wstring& name, const std::variant<float, int, bool, std::wstring>& value);

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
		Color backgroundColor{ 0X00000000 };
		Color hoverBackgroundColor{ 0X00000000 };
		HCURSOR cursor{ nullptr };
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


