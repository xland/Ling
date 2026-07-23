#include <winrt/Windows.UI.Composition.h>
#include <winrt/Windows.Foundation.Collections.h>
#include "WindowWithScroller.h"
#include "include/Ling.h"
#include "include/NodeScroller.h"
#include "include/Color.h"

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

    titleBar = std::make_unique<Ling::Node>(body.get());
    titleBar->setWidthPercent(100.f);
    titleBar->setHeight(30.f * dpi);
    titleBar->setBg(0xF8F8F8FF);

    initScrollerBox();
    show();
}

void WindowWithScroller::initScrollerBox()
{
    scrollerBox = std::make_unique<Ling::NodeScroller>(body.get());
    scrollerBox->setWidthPercent(100.f);
    scrollerBox->setFlexGrow(1.f);
    scrollerBox->setContentHeight(2 * h);
    auto linearBrush = compositor.CreateLinearGradientBrush();
    linearBrush.StartPoint({ 0.5f, 0.0f });
    linearBrush.EndPoint({ 0.5f, 1.0f });
    auto stop1 = compositor.CreateColorGradientStop(0.0f, Ling::Color(0x00FFFFFF).getUIColor());
    auto stop2 = compositor.CreateColorGradientStop(1.0f, Ling::Color(0xFFFF00FF).getUIColor());
    linearBrush.ColorStops().Append(stop1);
    linearBrush.ColorStops().Append(stop2);
    scrollerBox->visualContent.Brush(linearBrush);
}