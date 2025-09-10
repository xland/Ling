#include "../Ling/Include/WindowFrameless.h"
#include "../Ling/Include/WindowNormal.h"
#include "GraphicsDemo.h"

GraphicsDemo::GraphicsDemo()
{
    //win = std::make_unique<WindowFrameless>();
    win = std::make_unique<WindowNormal>();
    win->setTitle(L"SkiaGUI 示例窗口");
    win->setWindowSize(1000, 800);
    win->setWindowToScreenCenter();
    win->setBackgroundColor(0x234567FF);
    win->createNativeWindow();
}
GraphicsDemo::~GraphicsDemo()
{

}

void GraphicsDemo::start()
{
    win->setFlexDirection(FlexDirection::Row);
    win->setFlexWrap(Wrap::Wrap);

    auto rect = new Element();
    rect->setSize(100, 100);
    rect->setBackgroundColor(0x876543FF);
    rect->setMargin(30);
    win->addChild(rect);

    auto roundRect = new Element();
    roundRect->setSize(100, 100);
    roundRect->setBackgroundColor(0x876543FF);
    roundRect->setMargin(30);
    roundRect->setRadius(12);
    win->addChild(roundRect);

    auto circle = new Element();
    circle->setSize(100, 100);
    circle->setBackgroundColor(0x876543FF);
    circle->setMargin(30);
    circle->setRadius(50);
    win->addChild(circle);

    win->layout();
    win->show();
}

