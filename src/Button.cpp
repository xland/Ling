#include "pch.h"
#include "../include/Button.h"
#include "../include/WinBase.h"

namespace Ling {

	Button::Button(WinBase* win) :Node(win), EventBase()
	{
		setJustifyContent(Ling::Justify::Center);
		setAlignItems(Ling::Align::Center);
		label = makeChild<Ling::NodeText>();
		onMoveId = win->on(Ling::Event::MouseMove, [this](void* e) { this->onMove(e); });
		onDownId = win->on(Ling::Event::MouseDown, [this](void* e) { this->onDown(e); });
	}

	Button::~Button()
	{
		win->off(Ling::Event::MouseMove, onMoveId);
		win->off(Ling::Event::MouseDown, onDownId);
	}
	void Button::setText(const std::wstring& text)
	{
		label->setText(text);
	}
	void Button::setFontSize(float val)
	{
		label->setFontSize(val);
	}
	void Button::setFontFamily(const std::wstring& val)
	{
		label->setFontFamily(val);
	}
	void Button::setColor(Color color)
	{
		label->setColor(color);
	}
	void Button::setHoverColor(Color color)
	{
		hoverColor = color;
	}
	void Button::setHoverBg(Color color)
	{
		hoverBg = color;
	}
	void Button::onMove(void* e)
	{
		auto tuplePtr = static_cast<std::tuple<POINT>*>(e);
		auto [pos] = *tuplePtr;
		auto hoverFlag = isPosIn(pos);
		if (isHover != hoverFlag) {
			isHover = hoverFlag;
			if (isHover) {
				setBg(hoverBg);
				label->setColor(hoverColor);
			}
			else {
				setBg(hoverBg);
				label->setColor(label->color);
			}
		}
	}
	void Button::onDown(void* e)
	{
	}
}
