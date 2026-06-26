#include "ButtonIcon.h"
#include "WindowBase.h"
namespace Ling {
	ButtonIcon::ButtonIcon(WindowBase* win) :Element(win)
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
	void ButtonIcon::setIcon(const std::wstring& code)
	{
		this->code = code;
	}
	void ButtonIcon::setcolorFore(const Color& color)
	{
		colorFore = color;
	}
	void ButtonIcon::setcolorHoverFore(const Color& color)
	{
		colorHoverFore = color;
	}
	void ButtonIcon::setcolorHoverBackground(const Color& color)
	{
		colorHoverBackground = color;
	}
	void ButtonIcon::setFontSize(const float& fontSize)
	{
		this->fontSize = fontSize;
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
