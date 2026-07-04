#pragma once
#include <vector>
#include <memory>
#include <Windows.h>
#include <yoga/Yoga.h>
#include <winrt/Windows.UI.Composition.h>
#include "Element.h"
namespace Ling {
	using namespace winrt::Windows::UI;
	class Text;
	class ButtonIcon;
	class Box;
	class WindowElementManager
	{
	public:
		WindowElementManager();
		virtual ~WindowElementManager();
		void layout(const float& w,const float& h);
	public:
		std::unique_ptr<Box> body;
		std::vector<std::unique_ptr<Element>> elements;
		Composition::Compositor compositor;
	protected:
		void initBody();
		void mouseEnterElement(const int& x,const int& y);
		void mouseLeaveElement();
		BOOL setCursor();
	protected:
		Element* hoverElement;
	private:
	};
}