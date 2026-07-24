#include "pch.h"
#include "WindowImage.h"

WindowImage::WindowImage():Ling::WinBase()
{
    setTitle(L"图像控件演示");
    setSize(800, 600);
    setCenter();
    on(Ling::Event::Destroy, [this](void* e) { Ling::App::get()->quit(); });
}

WindowImage::~WindowImage()
{
}

void WindowImage::onCreated()
{
    enableShadow();
    body->setBg(0xFFFFFFFF);
    body->setFlexDirection(Ling::FlexDirection::Column);
    titleBar = std::make_unique<TitleBar>(this);
    show();
}

LRESULT WindowImage::onHitTest(const POINT pos)
{
    POINT pt = pos;
    ScreenToClient(hwnd, &pt);
    if (!isMaximized) {
        auto result = borderHitTest(pt);
        if (result != HTCLIENT) return result;
    }
    return titleBar->hitCaption(pt);
}
