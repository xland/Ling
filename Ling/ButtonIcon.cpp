#include "ButtonIcon.h"
#include "WindowBase.h"
namespace Ling {
	ButtonIcon::ButtonIcon(WindowBase* win) :Element<ButtonIcon>(win)
	{
		iconText = win->makeText();
		//onMouseEnter([this](const auto& e){
		//	visual.Brush(this->win->compositor.CreateColorBrush(colorHoverBackground.getUIColor()));
		//});
		//onMouseLeave([this](const auto& e) {
		//	visual.Brush(this->win->compositor.CreateColorBrush(backgroundColor.getUIColor()));
	 //    });
	}
	ButtonIcon::~ButtonIcon()
	{

	}
	ButtonIcon* ButtonIcon::setIcon(const std::wstring& code)
	{
		this->code = code;
		return this;
	}
	ButtonIcon* ButtonIcon::setcolorFore(const Color& color)
	{
		colorFore = color;
		return this;
	}
	ButtonIcon* ButtonIcon::setcolorHoverFore(const Color& color)
	{
		colorHoverFore = color;
		return this;
	}
	ButtonIcon* ButtonIcon::setcolorHoverBackground(const Color& color)
	{
		colorHoverBackground = color;
		return this;
	}
	ButtonIcon* ButtonIcon::setFontSize(const float& fontSize)
	{
		this->fontSize = fontSize;
		return this;
	}
	void ButtonIcon::initProperty()
	{
		visual.Brush(win->compositor.CreateColorBrush(backgroundColor.getUIColor()));
		setJustifyContent(Ling::Justify::Center);
		setAlignItems(Ling::Align::Center);
		setCursor(IDC_HAND);

		iconText->setForeColor(colorFore);
		iconText->setText(code);
		iconText->setFontSize(fontSize);
		iconText->setFontFamily(L"icon");
		addChild(iconText);
	}
}
