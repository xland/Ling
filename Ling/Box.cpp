#include "Box.h"
#include "WindowElementManager.h"
namespace Ling {
	Box::Box(WindowBase* win) :Element<Box>(win)
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
