#include "pch.h"
#include <Windows.h>
#include "WindowImage.h"

WindowImage::WindowImage():Ling::WinBase()
{
    setTitle(L"图像控件演示");
    setSize(800, 600);
    setCenter();
    onDestroy.add([this] { Ling::App::get()->quit(); });
    onMouseDown.add([this](POINT pos, bool flag ) { this->onDown(pos,flag); });
    createNativeWindow();
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
    label = body->makeChild<Ling::Label>();
    label->setText(L"点击加载图像");
    label->setFontSize(36.f);
    label->setColor(0x888888FF);
    label->setFlexGrow(1.f);
    label->setJustifyContent(Ling::Justify::Center);
    label->setAlignItems(Ling::Align::Center);
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

void WindowImage::onDown(POINT pos,bool isRight)
{
    if (pos.y < titleBar->height*dpi) return;
    COMDLG_FILTERSPEC types[] = {
        { L"png", L"*.png" },
        { L"jpeg", L"*.jpg;*.jpeg" },
        { L"all files", L"*.*" }
    };
    auto imgPath = openFileDialog(types);
    if (imgPath.empty()) return;
    if (!img) {
        img = body->makeChild<Ling::ImageBox>();
        img->setFlexGrow(1.f);      // 让 ImageBox 吃掉按钮以外的剩余空间
    }
    label->hide();
    img->loadImg(imgPath);
}
