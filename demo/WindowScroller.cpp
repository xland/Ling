#include "pch.h"
#include "WindowScroller.h"
#include "TitleBar.h"

WindowScroller::WindowScroller():Ling::WinBase()
{
    setTitle(L"滚动容器演示");
    setSize(800, 600);
    setCenter();
    onDestroy.add([this] { Ling::App::get()->quit(); });
    createNativeWindow();
}

WindowScroller::~WindowScroller()
{
}

void WindowScroller::onCreated()
{
    enableShadow();
    body->setBg(0xFFFFFFFF);
    body->setFlexDirection(Ling::FlexDirection::Column);
    titleBar = std::make_unique<TitleBar>(this);
    initScrollerBox();
    show();
}

LRESULT WindowScroller::onHitTest(const POINT pos)
{
    POINT pt = pos;
    ScreenToClient(hwnd, &pt); 
    if (!isMaximized) {
        auto result = borderHitTest(pt);
        if (result != HTCLIENT) return result;
    }
    return titleBar->hitCaption(pt);
}

void WindowScroller::initScrollerBox()
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