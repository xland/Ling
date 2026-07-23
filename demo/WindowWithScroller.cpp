#include <winrt/Windows.UI.Composition.h>
#include <winrt/Windows.Foundation.Collections.h>

#include "WindowWithScroller.h"
#include "include/Ling.h"

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
    enableShadow();
    body->setBg(0xFFFFFFFF);
    body->setFlexDirection(Ling::FlexDirection::Column); // 默认主轴就是 Column（垂直方向），这里显式设置更清晰

    auto titleBar = body->makeChild<Ling::Node>();
    titleBar->setWidthPercent(100.f);
    titleBar->setHeight(30.f);
    titleBar->setBg(0xF8F8F8FF);
    titleBar->setFlexDirection(Ling::FlexDirection::Row);

    titleBox = titleBar->makeChild<Ling::Node>();
    titleBox->setJustifyContent(Ling::Justify::Center);
    titleBox->setPaddingLeft(12.f);
    titleBox->setFlexGrow(1.f);

    auto titleText = titleBox->makeChild<Ling::NodeText>();
    titleText->setText(L"窗口标题");

    std::vector<std::wstring> iconCodes = { L"\ue6e8",L"\ue6e8",L"\ue6e8" };

    for (size_t i = 0; i < iconCodes.size(); i++)
    {
        auto btn = titleBar->makeChild<Ling::Node>();
        btn->setJustifyContent(Ling::Justify::Center);
        btn->setAlignItems(Ling::Align::Center);
        btn->setBg(0xCCCCCCFF);
        btn->setWidth(32.f);
        auto icon = btn->makeChild<Ling::NodeText>();
        icon->setText(iconCodes[i], 13.f, L"icon");
    }



    initScrollerBox();
    show();
}

LRESULT WindowWithScroller::onHitTest(const POINT& pos)
{
    POINT pt = pos;
    ScreenToClient(hwnd, &pt);
    // 3) 边框命中区厚度（逻辑像素 → 物理像素）
    const int border = static_cast<int>(4 * dpi);
    if (!wasMaximized) {
        const bool onLeft = pt.x >= 0 && pt.x < border;
        const bool onRight = pt.x <= w && pt.x >= w - border;
        const bool onTop = pt.y >= 0 && pt.y < border;
        const bool onBottom = pt.y <= h && pt.y >= h - border;

        // 四个角（角块是 border × border 的正方形，必须先于四边判断）
        if (onTop && onLeft)  return HTTOPLEFT;
        if (onTop && onRight) return HTTOPRIGHT;
        if (onBottom && onLeft)  return HTBOTTOMLEFT;
        if (onBottom && onRight) return HTBOTTOMRIGHT;

        // 四条边
        if (onLeft)   return HTLEFT;
        if (onRight)  return HTRIGHT;
        if (onTop)    return HTTOP;
        if (onBottom) return HTBOTTOM;
    }
    if (titleBox) {
        if (pt.y >= 0 && pt.y < titleBox->h && pt.x < titleBox->w) {
            return HTCAPTION;
        }
    }
    return HTCLIENT;
}

void WindowWithScroller::initScrollerBox()
{
    scrollerBox = body->makeChild<Ling::NodeScroller>();
    scrollerBox->setWidthPercent(100.f);
    scrollerBox->setFlexGrow(1.f);
    //scrollerBox->setHeight(900.f);
    // h 是窗口的物理像素高；content 高度想要"窗口物理高的 2 倍"，
    // 换算成逻辑像素传给 setContentHeight（内部会再乘 dpi）。
    scrollerBox->setContentHeight(2 * h / dpi);
    auto linearBrush = compositor.CreateLinearGradientBrush();
    linearBrush.StartPoint({ 0.5f, 0.0f });
    linearBrush.EndPoint({ 0.5f, 1.0f });
    auto stop1 = compositor.CreateColorGradientStop(0.0f, Ling::Color(0x00FFFFFF).getUIColor());
    auto stop2 = compositor.CreateColorGradientStop(1.0f, Ling::Color(0xFFFF00FF).getUIColor());
    linearBrush.ColorStops().Append(stop1);
    linearBrush.ColorStops().Append(stop2);
    scrollerBox->content->visual.Brush(linearBrush);
}