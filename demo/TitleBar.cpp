#include "pch.h"
#include "TitleBar.h"

TitleBar::TitleBar(Ling::WinBase* win):win{win}
{
    auto titleBar = win->body->makeChild<Ling::Node>();
    titleBar->setWidthPercent(100.f);
    titleBar->setHeight(30.f);
    titleBar->setBg(0xF8F8F8FF);
    titleBar->setFlexDirection(Ling::FlexDirection::Row);

    titleBox = titleBar->makeChild<Ling::Node>();
    titleBox->setJustifyContent(Ling::Justify::Center);
    titleBox->setAlignItems(Ling::Align::FlexStart);
    titleBox->setPaddingLeft(12.f);
    titleBox->setFlexGrow(1.f);

    auto titleText = titleBox->makeChild<Ling::Label>();
    titleText->setText(win->title);

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

    win->on(Ling::Event::Maximize, [this](void* e) { this->btns[1]->setText(L"\ue6e9"); });
    win->on(Ling::Event::Restore, [this](void* e) { this->btns[1]->setText(L"\ue6e5"); });
}

TitleBar::~TitleBar()
{

}

LRESULT TitleBar::hitCaption(const POINT pt)
{
    if (pt.y >= 0 && pt.y < titleBox->h && pt.x < titleBox->w) {
        return HTCAPTION;
    }
    return HTCLIENT;
}

void TitleBar::onDown(void* e)
{
    auto btn = (Ling::Button*)e;
    if (btn == btns[0]) {
        win->minimize();
    }
    else if (btn == btns[1]) {
        if (win->isMaximized) {
            win->restore();
        }
        else {
            win->maximize();
        }
    }
    else if (btn == btns[2]) {
        win->close();
    }
}