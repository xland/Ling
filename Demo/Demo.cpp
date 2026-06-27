#include "Demo.h"
#include "Ling/WindowBase.h"
#include "Ling/Text.h"
#include "Ling/ButtonIcon.h"
#include "Ling/Box.h"
Demo::Demo()
{

	setTitle(L"Ling 示例窗口");
	setSize(1000, 800);
	setPosScreenCenter();
	createNativeWindow(0, WS_OVERLAPPEDWINDOW); //必须先创建窗口，再创建元素

	auto top = makeBox()
		->setId(L"top")
		->setSize(SizeNaN, 40.f)
		->setBackgroundColor(0xDDDDDDFF)
		->setFlexDirection(Ling::FlexDirection::Row)
		->setJustifyContent(Ling::Justify::End);
	auto btnIcon = makeButtonIcon()
		->setId(L"btnIcon")
		->setSize(42.f, 40.f)
		->setIcon(L"\ue62d")
		->setcolorFore(0xDD22DDFF)
		->setBackgroundColor(0x99DDDDFF)
		->setHoverBackgroundColor(0x99DD22FF);
	btnIcon->onMouseDown([](const auto& e) {
		OutputDebugString(L"allen");
	});
	
	top->addChild(btnIcon);
	body->setFlexDirection(Ling::FlexDirection::Column);
	body->addChild(top);
	show();
}

Demo::~Demo()
{
}

