#include <WindowNormal.h>

#include "EventDemo.h"

EventDemo::EventDemo()
{
    win = std::make_unique<WindowNormal>();
    win->setTitle(L"SkiaGUI 示例窗口");
    win->setWindowSize(1000, 800);
    win->setWindowToScreenCenter();
    win->setBackgroundColor(0xFF234567);
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
        ele->setBackgroundColor(0xFF000000);
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
        ele->setBackgroundColor(0xFFFFFF00);
        ele->update();
        });
    win->setAlignItems(Align::Center);
    win->setJustifyContent(Justify::Center);
    win->addChild(ele);
    win->layout();    
    win->show();
}
