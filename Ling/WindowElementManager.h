#pragma once
#include "Header.h"
#include <yoga/Yoga.h>
#include "IElement.h"
namespace Ling {
	class Text;
	class ButtonIcon;
	class Box;
	class WindowElementManager
	{
	public:
		WindowElementManager();
		virtual ~WindowElementManager();
		Box* makeBox();
		Text* makeText();
		ButtonIcon* makeButtonIcon();
		void layout(const float& w,const float& h);
	public:
		std::unique_ptr<Box> body;
		std::vector<std::unique_ptr<IElement>> elements;
		Composition::Compositor compositor;
	protected:
		void initBody();
		void mouseEnterElement(const int& x,const int& y);
		void mouseLeaveElement();
		BOOL setCursor();
	private:
		IElement* hoverElement;
	};
}