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
	createNativeWindow(0, WS_POPUP| WS_MAXIMIZEBOX | WS_MINIMIZEBOX); //必须先创建窗口，再创建元素
	enableShadow();
	auto titleBarH{ 36.f };
	auto top = makeBox()
		->setId(L"top")
		->setSize(SizeNaN, titleBarH)
		->setBackgroundColor(0xDDDDDDFF)
		->setFlexDirection(Ling::FlexDirection::Row);

	titleBox = makeBox()
		->setSize(SizeNaN, titleBarH)
		->setFlexGrow(1.f)
		->setJustifyContent(Ling::Justify::Center)
		->setPaddingLeft(14.f);
	top->addChild(titleBox);

	auto title = makeText()
		->setText(L"窗口标题")
		->setForeColor(0x000000ff)
		->setFontSize(14.f)
		->setFlexShrink(1.f);
	titleBox->addChild(title);

	auto btnIcon = makeButtonIcon()
		->setId(L"btnIcon")
		->setSize(42.f, titleBarH)
		->setIcon(L"\ue62d")
		->setcolorFore(0xDD22DDFF)
		->setHoverBackgroundColor(0x99DD22FF);
	btnIcon->onMouseDown([title](const auto& e) {
		title->setText(L"这是修改过的标题！！！");
		OutputDebugString(L"allen");
	});	
	top->addChild(btnIcon);

	body->setFlexDirection(Ling::FlexDirection::Column);
	body->addChild(top);
	show();
	layout(w, h);
}

Demo::~Demo()
{
}

LRESULT Demo::onHitTest(const int& x, const int& y)
{
	if (x > 0 && x < 100 && y>0 && y < 60) {
		return HTCAPTION;
	}
	//if (hoverElement == titleBox) {
	//	return HTCAPTION;
	//}
	else {
		return HTCLIENT;
	}
}

