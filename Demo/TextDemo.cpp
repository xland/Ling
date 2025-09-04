#include "../Ling/Include/WindowNormal.h"
#include "../Ling/Include/TextBlock.h"
#include "../Ling/Include/Label.h"
#include "TextDemo.h"



TextDemo::TextDemo()
{
    win = std::make_unique<WindowNormal>();
    win->setTitle(L"Ling 示例窗口");
    win->setWindowSize(1000, 800);
    win->setWindowToScreenCenter();
    win->setBackgroundColor(0xFFDDDDDD);
    win->createNativeWindow();
}

TextDemo::~TextDemo()
{
}

void TextDemo::label()
{
    auto ele = new Label();
    ele->setText(u8R"(醉里挑灯看剑，梦回吹角连营。Abc, Def,)");
    ele->setFont("Microsoft YaHei");//STSong
    ele->setFontSize(21);
    ele->setBackgroundColor(0xFFFFFFFF);

    win->setAlignItems(Align::Center);
    win->setJustifyContent(Justify::Center);
    win->addChild(ele);
}

void TextDemo::textblock()
{

    auto ele = new TextBlock();
    ele->setText(u8R"(醉里挑灯看剑，梦回吹角连营。
    八百里分麾下炙，五十弦翻塞外声。
    沙场秋点兵。
    马作的卢飞快，弓如霹雳弦惊。
    了却君王天下事，赢得生前身后名。
    可怜白发生！)");
    ele->setFont("Microsoft YaHei");//STSong
    ele->setFontSize(21);
    ele->setBackgroundColor(0xFFFFFFFF);
    win->setAlignItems(Align::Center);
    win->setJustifyContent(Justify::Center);
    win->addChild(ele);
}

void TextDemo::start() {
    textblock();
    //label();
    win->layout();
    win->show();
}
