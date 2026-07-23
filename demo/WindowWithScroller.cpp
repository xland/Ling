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
    on(Ling::Event::SizeChanged, [this](void* arg) {this->onSize(arg);});
    body->setBg(0xFFFFFFFF);
    scrollerBox = new Ling::NodeScroller(body.get());    
    scrollerBox->setSize(w, h);
    scrollerBox->setContentHeight(2 * h);

    auto linearBrush = compositor.CreateLinearGradientBrush();
    linearBrush.StartPoint({ 0.5f, 0.0f });
    linearBrush.EndPoint({ 0.5f, 1.0f });
    auto stop1 = compositor.CreateColorGradientStop(0.0f, Ling::Color(0x00FFFFFF).getUIColor());
    auto stop2 = compositor.CreateColorGradientStop(1.0f, Ling::Color(0xFFFF00FF).getUIColor());
    linearBrush.ColorStops().Append(stop1);
    linearBrush.ColorStops().Append(stop2);

    scrollerBox->visualContent.Brush(linearBrush);

    show();
}

void WindowWithScroller::onSize(void* arg)
{


}
