#include "WindowWithScroller.h"
#include "include/Node.h"

WindowWithScroller::WindowWithScroller():Ling::WinBase()
{
    setTitle(L"图像文字识别工具");
    setSize(800, 600);
    setCenter();
}

WindowWithScroller::~WindowWithScroller()
{
}

void WindowWithScroller::onCreated()
{
    on(Ling::EventType::SizeChanged, [this](void* arg) {this->onSize(arg);});
    body->setBg(0xFFFFFFFF);
    scrollerBox = createNodeScroller("scrollerBox");    
    enableShadow();
    show();
    onSize(nullptr);
}

void WindowWithScroller::onSize(void* arg)
{
    scrollerBox->setPosSize(0, 0, w, h);
    scrollerBox->setContentHeight(2 * h);
}
