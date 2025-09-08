#include "../Ling/Include/WindowFrameless.h"
#include "../Ling/Include/WindowNormal.h"

#include "LayoutDemo.h"

LayoutDemo::LayoutDemo() 
{
    win = std::make_unique<WindowFrameless>();
    //win = std::make_unique<WindowNormal>();
    win->setTitle(L"SkiaGUI 示例窗口");
    win->setWindowSize(1000, 800);
    win->setWindowToScreenCenter();
    win->setBackgroundColor(0x234567FF);
    win->createNativeWindow();
}
LayoutDemo::~LayoutDemo() 
{

}

void LayoutDemo::leftRight()
{
    auto left = new Element();
    left->setSize(300, SizeNaN);
    left->setBackgroundColor(0xFF876543);
    left->setCaptionFlag(true);
    win->addChild(left);

    auto right = new Element();
    right->setFlexGrow(1.f);
    right->setFlexShrink(1.f);
    right->setBackgroundColor(0xFF9988aa);
    win->addChild(right);

    win->setFlexDirection(FlexDirection::Row);
}

void LayoutDemo::topCenterBottom()
{
    auto top = new Element();
    top->setSize(SizeNaN, 80);
    top->setBackgroundColor(0xFF876543);
    top->setCaptionFlag(true);
    win->addChild(top);

    auto center = new Element();
    center->setFlexGrow(1.f);
    center->setFlexShrink(1.f);
    center->setBackgroundColor(0xFF9988aa);
    win->addChild(center);

    auto bottom = new Element();
    bottom->setSize(SizeNaN, 40);
    bottom->setBackgroundColor(0xFF313951);
    win->addChild(bottom);

    win->setFlexDirection(FlexDirection::Column);
}

void LayoutDemo::verticalHorizontalCenter()
{
    auto ele = new Element();
    ele->setSize(100, 100);
    ele->setBackgroundColor(0x876543FF);
    ele->setCaptionFlag(true);
    win->setAlignItems(Align::Center);
    win->setJustifyContent(Justify::Center);
    win->addChild(ele);
}

void LayoutDemo::start()
{
    //leftRight();
    //topCenterBottom();
    verticalHorizontalCenter();
    win->layout();
    win->show();
}
