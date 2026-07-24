#include "pch.h"
#include "WindowWithScroller.h"

WindowWithScroller::WindowWithScroller():Ling::WinBase()
{
    setTitle(L"图像文字识别工具");
    setSize(800, 600);
    setCenter();

    on(Ling::Event::MouseMove, [this](void* e) { this->onMove(e); });
    on(Ling::Event::MouseDown, [this](void* e) { this->onDown(e); });
    on(Ling::Event::Maximize, [this](void* e) { this->btns[1]->setText(L"\ue6e9"); });
    on(Ling::Event::Restore, [this](void* e) { this->btns[1]->setText(L"\ue6e5"); });
    on(Ling::Event::Destroy, [this](void* e) { Ling::App::get()->quit(); });
}

WindowWithScroller::~WindowWithScroller()
{
}

void WindowWithScroller::onCreated()
{
    enableShadow();
    body->setBg(0xFFFFFFFF);
    body->setFlexDirection(Ling::FlexDirection::Column); // 默认主轴就是 Column（垂直方向），这里显式设置更清晰
    initTitleBar();
    initScrollerBox();
    show();
}

LRESULT WindowWithScroller::onHitTest(const POINT& pos)
{
    POINT pt = pos;
    ScreenToClient(hwnd, &pt); 
    const int border = static_cast<int>(4 * dpi);
    if (!isMaximized) {
        const bool onLeft = pt.x >= 0 && pt.x < border;
        const bool onRight = pt.x <= w && pt.x >= w - border;
        const bool onTop = pt.y >= 0 && pt.y < border;
        const bool onBottom = pt.y <= h && pt.y >= h - border; 
        if (onTop && onLeft)  return HTTOPLEFT;
        if (onTop && onRight) return HTTOPRIGHT;
        if (onBottom && onLeft)  return HTBOTTOMLEFT;
        if (onBottom && onRight) return HTBOTTOMRIGHT; 
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

void WindowWithScroller::initTitleBar()
{
    auto titleBar = body->makeChild<Ling::Node>();
    titleBar->setWidthPercent(100.f);
    titleBar->setHeight(30.f);
    titleBar->setBg(0xF8F8F8FF);
    titleBar->setFlexDirection(Ling::FlexDirection::Row);

    titleBox = titleBar->makeChild<Ling::Node>();
    titleBox->setJustifyContent(Ling::Justify::Center);
    titleBox->setAlignItems(Ling::Align::FlexStart);
    titleBox->setPaddingLeft(12.f);
    titleBox->setFlexGrow(1.f);

    auto titleText = titleBox->makeChild<Ling::NodeText>();
    titleText->setText(title);

    std::vector<std::wstring> iconCodes = { L"\ue6e8",L"\ue6e5",L"\ue6e7" };
    for (size_t i = 0; i < iconCodes.size(); i++)
    {
        auto btn = titleBar->makeChild<Ling::Button>();
        btn->setId(std::format("btn{}", i));
        btn->setWidth(32.f);
        btn->setText(iconCodes[i]);
        btn->setFontFamily(L"icon");
        btn->setColor(0X888888FF);
        if (i == 2) {
            btn->setHoverColor(0xFFFFFFff);
            btn->setHoverBg(0xE81123FF);
        }
        else {
            btn->setHoverColor(0X333333FF);
            btn->setHoverBg(0xE6E6E6FF);
        }
        btn->setFontSize(12.f);
        btn->on(Ling::Event::MouseDown, [this](auto arg) {this->onDown(arg);});
        btns.push_back(btn);
    }
}

void WindowWithScroller::initScrollerBox()
{
    scrollerBox = body->makeChild<Ling::NodeScroller>();
    scrollerBox->setWidthPercent(100.f);
    scrollerBox->setFlexGrow(1.f);
    for (size_t i = 0; i < 100; i++)
    {
        auto text = scrollerBox->makeChild<Ling::NodeText>();
        text->setText(std::format(L"这是一行文本，看看能渲染多少条{}", i));        
        text->setMargin(12.f, 8.f, 12.f, 8.f);
    }
    


    //auto linearBrush = compositor.CreateLinearGradientBrush();
    //linearBrush.StartPoint({ 0.5f, 0.0f });
    //linearBrush.EndPoint({ 0.5f, 1.0f });
    //auto stop1 = compositor.CreateColorGradientStop(0.0f, Ling::Color(0x00FFFFFF).getUIColor());
    //auto stop2 = compositor.CreateColorGradientStop(1.0f, Ling::Color(0xFFFF00FF).getUIColor());
    //linearBrush.ColorStops().Append(stop1);
    //linearBrush.ColorStops().Append(stop2);
    //scrollerBox->content->visual.Brush(linearBrush);
}

void WindowWithScroller::onMove(void* e)
{
    auto tuplePtr = static_cast<std::tuple<POINT>*>(e);
    auto [pos] = *tuplePtr;
    int index{ -1 };
    for (size_t i = 0; i < btns.size(); i++)
    {
        auto btn = btns[i];
        if (btn->isPosIn(pos)) {
            index = i;
            break;
        }
    }
    if (hoverTitleBtnIndex != index) {
        if (hoverTitleBtnIndex != -1) {
            btns[hoverTitleBtnIndex]->setBg(0x00000000);
        }
        if (index != -1) {
            btns[index]->setBg(0xE6E6E6FF);
        }
        hoverTitleBtnIndex = index;
    }
}

void WindowWithScroller::onDown(void* e)
{
    auto btn = (Ling::Button*)e;
    if (btn == btns[0]) {
        minimize();
    }
    else if (btn == btns[1]) {
        if (isMaximized) {
            restore();
        }
        else {
            maximize();
        }
    }
    else if (btn == btns[2]) {
        close();
    }
}
