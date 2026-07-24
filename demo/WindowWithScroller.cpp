#include "pch.h"
#include "WindowWithScroller.h"
#include "TitleBar.h"

WindowWithScroller::WindowWithScroller():Ling::WinBase()
{
    setTitle(L"图像文字识别工具");
    setSize(800, 600);
    setCenter();
    on(Ling::Event::Destroy, [this](void* e) { Ling::App::get()->quit(); });
}

WindowWithScroller::~WindowWithScroller()
{
}

void WindowWithScroller::onCreated()
{
    enableShadow();
    body->setBg(0xFFFFFFFF);
    body->setFlexDirection(Ling::FlexDirection::Column);
    titleBar = std::make_unique<TitleBar>(this);
    initScrollerBox();
    show();
}

LRESULT WindowWithScroller::onHitTest(const POINT pos)
{
    POINT pt = pos;
    ScreenToClient(hwnd, &pt); 
    if (!isMaximized) {
        auto result = borderHitTest(pt);
        if (result != HTCLIENT) return result;
    }
    return titleBar->hitCaption(pt);
}

void WindowWithScroller::initScrollerBox()
{
    scrollerBox = body->makeChild<Ling::ScrollerBox>();
    scrollerBox->setWidthPercent(100.f);
    scrollerBox->setFlexGrow(1.f);
    for (size_t i = 0; i < 100; i++)
    {
        auto text = scrollerBox->makeChild<Ling::Label>();
        text->setText(std::format(L"这是一行文本，看看能渲染多少条{}", i));        
        text->setMargin(12.f, 8.f, 12.f, 8.f);
    }
}