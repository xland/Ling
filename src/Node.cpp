#include "pch.h"
#include "../include/WinBase.h"
#include "../include/Node.h"

namespace Ling {

	Node::Node(WinBase* win):win{win}
	{
		visual = win->compositor.CreateSpriteVisual();
		//win->visual.Children().InsertAtTop(visual);
	}

	Node::~Node()
	{
	}

	bool Node::isPosIn(float x, float y)
	{
		return false;
	}

	void Node::setPosSize(const float& x, const float& y, const float& w, const float& h)
	{
		visual.Offset({ x,y,0.f });
		visual.Size({ w,h });
	}

	std::tuple<float, float, float, float> Node::getPosSize()
	{
		auto pos = visual.Offset();
		auto size = visual.Size();
		return std::make_tuple(pos.x,pos.y,size.x,size.y);
	}
	void Node::hide()
	{
		visual.IsVisible(false);
	}

	void Node::show()
	{
		visual.IsVisible(true);
	}

	void Node::setBg(const Color& color)
	{
		visual.Brush(win->compositor.CreateColorBrush(color.getUIColor()));
	}
}
