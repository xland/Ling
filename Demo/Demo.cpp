#include "Demo.h"
#include "Ling/App.h"
#include "Ling/WindowBase.h"
#include "Ling/Text.h"
#include "Ling/ButtonIcon.h"
#include "Ling/Box.h"
#include "Ling/Property.h"
Demo::Demo()
{
	std::wstring winTitle{ L"Ling 示例窗口" };
	setTitle(winTitle);
	setSize(1000, 800);
	setPosScreenCenter();
	createNativeWindow(0, WS_POPUP| WS_MAXIMIZEBOX | WS_MINIMIZEBOX); //必须先创建窗口，再创建元素
	enableShadow();

	auto prop = std::make_shared<Ling::Property>();
	//prop->onChangeFloat([this](const std::wstring& name, const float& val) {
	//	int a = 1;
	//});

	//prop->set(L"test", 1.f);

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
		->setText(winTitle)
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
	btnIcon->onMouseDown([this](const auto& e) {
		this->close();
		OutputDebugString(L"allen");
		Ling::App::get()->quit(0);
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
	if (titleBox->containPosition(x,y)) {
		return HTCAPTION;
	}
	else {
		return HTCLIENT;
	}
}

