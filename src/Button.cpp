#include "pch.h"
#include "../include/Button.h"
#include "../include/WinBase.h"
#include "../include/Text.h"

namespace Ling {

	Button::Button(WinBase* win) :Node(win)
	{
		setJustifyContent(Ling::Justify::Center);
		setAlignItems(Ling::Align::Center);
		text = makeChild<Text>();
		moveTok = win->onMouseMove.add([this](POINT pos) { this->onMove(pos); });
		downTok = win->onMouseDown.add([this](POINT pos, bool isRight) { this->onDown(pos, isRight); });
	}

	Button::~Button()
	{
		win->onMouseMove.remove(moveTok);
		win->onMouseDown.remove(downTok);
	}
	void Button::setText(const std::wstring& s)
	{
		text->setText(s);
	}
	void Button::setFontSize(float val)
	{
		text->setFontSize(val);
	}
	void Button::setFontFamily(const std::wstring& val)
	{
		text->setFontFamily(val);
	}
	void Button::setColor(Color color)
	{
		text->setColor(color);
		this->color = color;
	}
	void Button::setBg(const Color& color)
	{
		bgColor = color;
		normalBrush = win->compositor.CreateColorBrush(color.getUIColor());
		if (!isHover) visual.Brush(normalBrush);
	}
	void Button::setHoverColor(Color color)
	{
		hoverColor = color;
	}
	void Button::setHoverBg(Color color)
	{
		hoverBg = color;
		hoverBrush = win->compositor.CreateColorBrush(color.getUIColor());
		if (isHover) visual.Brush(hoverBrush);
	}
	void Button::onMove(POINT pos)
	{
		auto hoverFlag = isPosIn(pos);
		if (isHover == hoverFlag) return;
		isHover = hoverFlag;
		// 背景色如果配置一致则不需要换 brush；text->setColor 内部已判等，可放心调。
		if (isHover) {
			visual.Brush(hoverBrush);
			text->setColor(hoverColor);
		}
		else {
			visual.Brush(normalBrush);
			text->setColor(color);
		}
	}
	void Button::onDown(POINT pos, bool isRight)
	{
		if (!isRight && isPosIn(pos)) {
			onClick(this);
		}
	}
}
