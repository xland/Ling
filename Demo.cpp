#include "Demo.h"
#include "Ling/WindowBase.h"
#include "Ling/Screen.h"
#include "Ling/Text.h"
#include "Ling/ButtonIcon.h"
Demo::Demo()
{
	setTitle(L"Ling 示例窗口");
	setSize(1000, 800);
	setPosScreenCenter();
	createNativeWindow(0, WS_OVERLAPPEDWINDOW); //必须先创建窗口，再创建元素

	auto top = makeElement();
	top->setSize(SizeNaN, 40.f);
	top->setBackgroundColor(0xDDDDDDFF);
	top->setFlexDirection(Ling::FlexDirection::Row);
	top->setJustifyContent(Ling::Justify::End);

	auto btnIcon = makeButtonIcon();
	btnIcon->setSize(42.f, 40.f);
	btnIcon->setIcon(L"\ue62d");
	btnIcon->setcolorFore(0xDD22DDFF);
	btnIcon->setBackgroundColor(0x99DDDDFF);
	top->addChild(btnIcon);

	//auto closeBtn = makeElement();
	//closeBtn->setSize(42.f, 40.f);
	//closeBtn->setJustifyContent(Ling::Justify::Center);
	//closeBtn->setAlignItems(Ling::Align::Center);
	//closeBtn->setCursor(IDC_HAND);

	//auto text = makeText();
	//text->setForeColor(0x8800FFff);
	//text->setText(L"\ue62d");
	//text->setFontSize(18.f);
	//text->setFontFamily(L"icon");//icon SimSun Microsoft YaHei
	//closeBtn->addChild(text);
	//top->addChild(closeBtn);
	
	//auto body = std::make_shared<Ling::Element>();
	//body->setFlexGrow(1.f);
	//body->setFlexShrink(1.f);
	//body->setBackgroundColor(0x9988aaFF);
	//win->addChild(body);



	body->setFlexDirection(Ling::FlexDirection::Column);


	//auto ele = std::make_shared<Element>();
	//ele->setSize(100, 100);
	//addChild(ele);
	body->addChild(top);
	show();
}

Demo::~Demo()
{
}

