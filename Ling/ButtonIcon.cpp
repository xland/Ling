#include "ButtonIcon.h"
#include "WindowBase.h"
#include "Text.h"
namespace Ling {
	ButtonIcon::ButtonIcon(WindowBase* win):Element(win)
	{

	}
	ButtonIcon::~ButtonIcon()
	{

	}
	void ButtonIcon::initProperty()
	{
		setJustifyContent(Ling::Justify::Center);
		setAlignItems(Ling::Align::Center);
		setCursor(IDC_HAND);

		auto text = win->makeText();
		text->setForeColor(0x8800FFff);
		text->setText(L"\ue62d");
		text->setFontSize(18.f);
		text->setFontFamily(L"icon");//icon SimSun Microsoft YaHei
		addChild(text);
	}
}