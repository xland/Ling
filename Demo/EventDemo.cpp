#include <format>
#include "../Ling/Include/WindowNormal.h"
#include "../Ling/Include/Element.h"
#include "../Ling/Include/ElementBox.h"
#include "../Ling/Include/Label.h"
#include "../Ling/Include/App.h"
#include "EventDemo.h"

EventDemo::EventDemo()
{
    win = std::make_unique<WindowNormal>();
    win->setTitle(L"Ling 示例窗口");
    win->setWindowSize(1000, 800);
    win->setWindowToScreenCenter();
    win->setBackgroundColor(0x234567FF);
    win->createNativeWindow();
}

EventDemo::~EventDemo()
{
}

void EventDemo::enterLeaveDownUp()
{
    auto ele = new Ling::Element();
    ele->setSize(120, 40);
    ele->setBackgroundColor(0xFFFFFFFF);

    ele->onMouseEnter([ele](const MouseEvent& event) {
        ele->setBackgroundColor(0x000000FF);
        ele->update();
        });
    ele->onMouseLeave([ele](const MouseEvent& event) {
        ele->setBackgroundColor(0xFFFFFFFF);
        ele->update();
        });
    ele->onMouseDown([ele](const MouseEvent& event) {
        ele->setBackgroundColor(0xFF00FFFF);
        ele->update();
        });
    ele->onMouseUp([ele](const MouseEvent& event) {
        ele->setBackgroundColor(0xFFFF00FF);
        ele->update();
        });

    win->addChild(ele);
}

void EventDemo::counter()
{
    App::loadSystemFont("SimHei");
    auto label = new Label();
    label->setFontSize(14);
    static int countNum{ 0 };
    auto str =std::format("点击我：{}次", countNum);
    std::u8string utf8_result{ str.begin(), str.end() };
    label->setText(u8"点击我：{}次");
    label->setFontName("SimHei");
    label->setForegroundColor(0X000000FF);
    label->setBackgroundColor(0xFFFFFFFF);
    label->onMouseEnter([label](const MouseEvent& event) {
        label->setBackgroundColor(0xDD0066FF);
        label->update();
        });
    label->onMouseLeave([label](const MouseEvent& event) {
        label->setBackgroundColor(0xFFFFFFFF);
        label->update();
        });
    label->onMouseDown([label](const MouseEvent& event) {
        label->setBackgroundColor(0xFF00FFFF);
        auto str = std::format("点击我：{}次", ++countNum);
        std::u8string utf8_result{ str.begin(), str.end() };
        label->setText(utf8_result);
        label->update();
    });
    win->addChild(label);
}

void EventDemo::start() {
    counter();
    win->setAlignItems(Align::Center);
    win->setJustifyContent(Justify::Center);
    win->layout();
    win->show();
}
