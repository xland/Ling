#pragma once
#include <functional>
#include <vector>
#include <unordered_map>
#include <Windows.h>
#include <yoga/Yoga.h>
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
	class IElement
	{
	public:
		IElement(WindowBase* win);
		virtual ~IElement();
		// 多态方法
		bool isAncestor(const IElement* target);
		IElement* findAncestor(IElement* other);
		void removeSelf();
		void removeChild(IElement* ele);
		void insertChild(const int& index, IElement* ele);
		void addChild(IElement* ele);
		virtual void layout();
		bool setCursor();
		IElement* hitTest(const int& x, const int& y);
		bool containPosition(const int& x, const int& y);

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
	public:
		float xAbs{ 0.f }, yAbs{ 0.f };
		WindowBase* win{ nullptr };
		Composition::SpriteVisual visual{ nullptr };
		YGNodeRef node{ nullptr };
		IElement* parent{ nullptr };
		std::wstring id;
	protected:
		virtual void initProperty() {};
	protected:
		std::vector<IElement*> children;
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


