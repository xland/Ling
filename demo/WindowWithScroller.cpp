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
    body->setBg(0xFFFFFFFF);
    enableShadow();
    show();
}
