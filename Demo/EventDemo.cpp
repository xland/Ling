#include "../Ling/Include/WindowNormal.h"
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

void EventDemo::start() {
    auto ele = new Element();
    ele->setSize(120, 40);
    ele->setBackgroundColor(0xFFFFFFFF);
    ele->onMouseEnter([ele](const MouseEvent& event) {
        ele->setBackgroundColor(0x000000FF);
        //ele->update();
        });
    ele->onMouseLeave([ele](const MouseEvent& event) {
        ele->setBackgroundColor(0xFFFFFFFF);
        //ele->update();
        });
    ele->onMouseDown([ele](const MouseEvent& event) {
        ele->setBackgroundColor(0xFF00FFFF);
        //ele->update();
        });
    ele->onMouseUp([ele](const MouseEvent& event) {
        ele->setBackgroundColor(0xFFFF00FF);
        //ele->update();
        });
    win->setAlignItems(Align::Center);
    win->setJustifyContent(Justify::Center);
    win->addChild(ele);
    win->layout();
    win->show();
}
