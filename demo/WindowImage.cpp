#include "pch.h"
#include <Windows.h>
#include "WindowImage.h"

WindowImage::WindowImage():Ling::WinBase()
{
    setTitle(L"图像控件演示");
    setSize(800, 600);
    setCenter();
    on(Ling::Event::MouseDown, [this](auto arg) {this->onDown(arg);});
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
    tipLabel = body->makeChild<Ling::Label>();
    tipLabel->setText(L"点击加载图像");
    tipLabel->setFontSize(36.f);
    tipLabel->setColor(0x888888FF);
    tipLabel->setFlexGrow(1.f);
    tipLabel->setJustifyContent(Ling::Justify::Center);
    tipLabel->setAlignItems(Ling::Align::Center);
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

void WindowImage::onDown(void* e)
{
    COMDLG_FILTERSPEC types[] = {
    { L"png", L"*.png" },
    { L"jpeg", L"*.jpg;*.jpeg" },
    { L"all files", L"*.*" }
    };
    auto path = openFileDialog(types);
}
