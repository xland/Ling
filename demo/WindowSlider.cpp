#include "pch.h"
#include "WindowSlider.h"

WindowSlider::WindowSlider() : Ling::WinBase()
{
    setTitle(L"Slider 滑块演示");
    setSize(800, 600);
    setCenter();
    onDestroy.add([this] { Ling::App::get()->quit(); });
    createNativeWindow();
}

WindowSlider::~WindowSlider()
{
}

void WindowSlider::onCreated()
{
    enableShadow();
    body->setBg(0xFFFFFFFF);
    body->setFlexDirection(Ling::FlexDirection::Column);
    titleBar = std::make_unique<TitleBar>(this);

    auto center = body->makeChild<Ling::Node>();
    center->setFlexGrow(1.f);
    center->setFlexDirection(Ling::FlexDirection::Column);
    center->setJustifyContent(Ling::Justify::Center);
    center->setAlignItems(Ling::Align::Center);

    // 连续滑块：0 - 100
    valueLabel = center->makeChild<Ling::Label>();
    valueLabel->setText(L"连续 (0-100)：0");
    valueLabel->setFontSize(16.f);
    valueLabel->setColor(0x333333FF);
    valueLabel->setMarginBottom(8.f);

    slider = center->makeChild<Ling::Slider>();
    slider->setWidth(320.f);
    slider->onValueChanged.add([this](Ling::Slider*, float v) {
        valueLabel->setText(std::format(L"连续 (0-100)：{:.1f}", v));
    });

    // 带 step 的滑块：-50 - 50，步进 10
    stepLabel = center->makeChild<Ling::Label>();
    stepLabel->setText(L"步进 10 (-50-50)：0");
    stepLabel->setFontSize(16.f);
    stepLabel->setColor(0x333333FF);
    stepLabel->setMarginTop(40.f);
    stepLabel->setMarginBottom(8.f);

    stepSlider = center->makeChild<Ling::Slider>();
    stepSlider->setWidth(320.f);
    stepSlider->setRange(-50.f, 50.f);
    stepSlider->setStep(10.f);
    stepSlider->setTrackColor(0xEEEEEEFF);
    stepSlider->setFillColor(0x10B981FF);
    stepSlider->setThumbColor(0x10B981FF);
    stepSlider->setHoverThumbColor(0x059669FF);
    stepSlider->onValueChanged.add([this](Ling::Slider*, float v) {
        stepLabel->setText(std::format(L"步进 10 (-50-50)：{:.0f}", v));
    });

    show();
}

LRESULT WindowSlider::onHitTest(const POINT pos)
{
    POINT pt = pos;
    ScreenToClient(hwnd, &pt);
    if (!isMaximized) {
        auto result = borderHitTest(pt);
        if (result != HTCLIENT) return result;
    }
    return titleBar->hitCaption(pt);
}
