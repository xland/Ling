#include "pch.h"
#include "Box.h"
#include "WindowBase.h"
namespace Ling {
	Box::Box(WindowBase* win) :Element(win)
	{
		//onMouseEnter([this](const auto& e){
		//	visual.Brush(this->win->compositor.CreateColorBrush(colorHoverBackground.getUIColor()));
		//});
		//onMouseLeave([this](const auto& e) {
		//	visual.Brush(this->win->compositor.CreateColorBrush(backgroundColor.getUIColor()));
	 //    });
	}
	Box::~Box()
	{

	}
	void Box::initProperty()
	{
		visual.Brush(win->compositor.CreateColorBrush(backgroundColor.getUIColor()));
	}
}