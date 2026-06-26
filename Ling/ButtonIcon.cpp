#include "ButtonIcon.h"
#include "WindowBase.h"
namespace Ling {
	ButtonIcon::ButtonIcon(WindowBase* win):Element(win)
	{
		iconText = win->makeText();
	}
	ButtonIcon::~ButtonIcon()
	{

	}
	void ButtonIcon::setIcon(const std::wstring& code)
	{
		this->code = code;
	}
	void ButtonIcon::initProperty()
	{
		setJustifyContent(Ling::Justify::Center);
		setAlignItems(Ling::Align::Center);
		setCursor(IDC_HAND);

		iconText->setForeColor(iconColorFore);
		iconText->setText(code);
		iconText->setFontSize(18.f);
		iconText->setFontFamily(L"icon");
		addChild(iconText);
	}
	void ButtonIcon::setIconColor(const Color& color)
	{
		iconColorFore = color;
		if (iconText) iconText->setForeColor(color);
	}

	void ButtonIcon::setIconFontSize(const float& fontSize)
	{
		if (iconText) iconText->setFontSize(fontSize);
	}
}
