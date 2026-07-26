#include "pch.h"
#include "WindowBorderRadius.h"

WindowBorderRadius::WindowBorderRadius() : Ling::WinBase()
{
    setTitle(L"圆角按钮演示");
    setSize(800, 600);
    setCenter();
    onDestroy.add([this] { Ling::App::get()->quit(); });
    createNativeWindow();
}

WindowBorderRadius::~WindowBorderRadius()
{
}

void WindowBorderRadius::onCreated()
{
    enableShadow();
    body->setBg(0xFFFFFFFF);
    body->setFlexDirection(Ling::FlexDirection::Column);
    titleBar = std::make_unique<TitleBar>(this);

    // 一个撑满标题栏之外空间的容器，用来把按钮居中放置
    auto center = body->makeChild<Ling::Node>();
    center->setFlexGrow(1.f);
    center->setJustifyContent(Ling::Justify::Center);
    center->setAlignItems(Ling::Align::Center);

    button = center->makeChild<Ling::Button>();
    button->setSize(160.f, 48.f);
    button->setText(L"圆角按钮");
    button->setFontSize(16.f);
    button->setColor(0xFFFFFFFF);
    button->setBg(0x3B82F6FF);           // 蓝色背景
    button->setHoverColor(0xFFFFFFFF);
    button->setHoverBg(0x2563EBFF);      // hover 时更深一档
    button->setBorderRadius(12.f);       // 圆角
    button->setBorder(2.f, 0x1D4ED8FF);  // 深蓝描边
    button->onClick.add([](Ling::Button*) {
        MessageBoxW(nullptr, L"按钮被点击了！", L"提示", MB_OK | MB_ICONINFORMATION);
    });

    show();
}

LRESULT WindowBorderRadius::onHitTest(const POINT pos)
{
    POINT pt = pos;
    ScreenToClient(hwnd, &pt);
    if (!isMaximized) {
        auto result = borderHitTest(pt);
        if (result != HTCLIENT) return result;
    }
    return titleBar->hitCaption(pt);
}
