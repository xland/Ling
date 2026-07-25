#include "pch.h"
#include "../include/Button.h"
#include "../include/WinBase.h"
#include "../include/Text.h"

namespace Ling {

	Button::Button(WinBase* win) :Node(win), EventBase()
	{
		setJustifyContent(Ling::Justify::Center);
		setAlignItems(Ling::Align::Center);
		text = makeChild<Text>();
		onMoveId = win->on(Ling::Event::MouseMove, [this](void* e) { this->onMove(e); });
		onDownId = win->on(Ling::Event::MouseDown, [this](void* e) { this->onDown(e); });
	}

	Button::~Button()
	{
		win->off(Ling::Event::MouseMove, onMoveId);
		win->off(Ling::Event::MouseDown, onDownId);
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
	void Button::onMove(void* e)
	{
		auto tuplePtr = static_cast<std::tuple<POINT>*>(e);
		auto [pos] = *tuplePtr;
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
	void Button::onDown(void* e)
	{
		auto tuplePtr = static_cast<std::tuple<POINT,bool>*>(e);
		auto [pos,isRight] = *tuplePtr;
		if (!isRight && isPosIn(pos)) {
			emit(Event::MouseDown,this);
		}
	}
}
