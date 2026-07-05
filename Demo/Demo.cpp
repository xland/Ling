#include "Demo.h"
#include "Ling/App.h"
#include "Ling/WindowBase.h"
#include "Ling/Text.h"
#include "Ling/Property.h"
Demo::Demo()
{
	std::wstring winTitle{ L"Ling 示例窗口" };
	setTitle(winTitle);
	setSize(1000, 800);
	setPosScreenCenter();
	createNativeWindow(0, WS_POPUP| WS_MAXIMIZEBOX | WS_MINIMIZEBOX); //必须先创建窗口，再创建元素
	enableShadow();

	auto titleBarH{ 36.f };
	auto prop = std::make_shared<Ling::Property>();
	prop->setSize(SizeNaN, titleBarH);
	prop->setBackgroundColor(0xDDDDDDFF);
	prop->setForegroundColor(0x000000FF);
	prop->setFlexDirection(Ling::FlexDirection::Row);
	prop->setFontFamily(L"Microsoft YaHei");
	prop->setFontSize(16.f);
	prop->setText(winTitle);
	auto title = makeText();
	title->setProperty(prop);
	body->addChild(title);
	show();
	layout(w, h);
}

Demo::~Demo()
{
}

LRESULT Demo::onHitTest(const int& x, const int& y)
{
	//if (titleBox->containPosition(x,y)) {
	//	return HTCAPTION;
	//}
	//else {
		return HTCLIENT;
	//}
}

