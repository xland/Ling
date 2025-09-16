#include "../Ling/Include/WindowFrameless.h"
#include "../Ling/Include/WindowNormal.h"
#include "../Ling/Include/EffectBox.h"
#include "GraphicsDemo.h"

GraphicsDemo::GraphicsDemo()
{
    //win = std::make_unique<WindowFrameless>();
    win = std::make_unique<WindowNormal>();
    win->setTitle(L"Ling 示例窗口");
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

    auto rect1 = new Element();
    rect1->setSize(100, 100);
    auto gradient = std::make_shared<Gradient>(GradientType::Linear);
    gradient->addColor(0.f, 0xFF0066FF);
    gradient->addColor(1.f, 0x0066FFFF);
    rect1->setBackgroundColor(gradient);
    rect1->setMargin(30);
    win->addChild(rect1);

    auto effect = new EffectBox();
    effect->setSize(100, 100);
    effect->setRadius(50);
    effect->setBackgroundColor(0x876543FF);
    effect->setShadow(0xFF0088FF, 0, 0, 30.f);
    effect->setMargin(30);
    win->addChild(effect);

    win->layout();
    win->show();
}

