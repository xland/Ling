#include "Demo.h"
#include "Ling/WindowBase.h"
#include "Ling/Screen.h"
Demo::Demo()
{
	setTitle(L"Ling 示例窗口");
	setSize(1000, 800);
	setPosScreenCenter();
	setBackgroundColor(0x234567FF);
	createNativeWindow(0, WS_OVERLAPPEDWINDOW);


	auto left = std::make_shared<Element>();
	left->setSize(300, SizeNaN);
	left->setBackgroundColor(0x876543FF);
	//left->setCaptionFlag(true);
	win->addChild(left);

	auto right = std::make_shared<Element>();
	right->setFlexGrow(1.f);
	right->setFlexShrink(1.f);
	right->setBackgroundColor(0x9988aaFF);
	win->addChild(right);

	win->setFlexDirection(Ling::FlexDirection::Row);


	//auto ele = std::make_shared<Element>();
	//ele->setSize(100, 100);
	//addChild(ele);
	show();
}

Demo::~Demo()
{
}

