#include "../Ling/Include/WindowNormal.h"
#include "../Ling/Include/TextArea.h"
#include "../Ling/Include/TextBlock.h"
#include "../Ling/Include/Label.h"
#include "../Ling/Include/App.h"
#include "../Ling/Include/Wrap.h"
#include "TextDemo.h"



TextDemo::TextDemo()
{
    win = std::make_unique<WindowNormal>();
    win->setTitle(L"Ling 示例窗口");
    win->setWindowSize(1000, 800);
    win->setWindowToScreenCenter();
    win->setBackgroundColor(0xDDDDDDFF);
    win->createNativeWindow();
}

TextDemo::~TextDemo()
{
}

void TextDemo::label()
{
    App::loadSystemFont("SimHei");
    auto ele = new Label();
    ele->setText(LR"(醉里挑灯看剑，梦回吹角连营。Abc, Def,)");
    ele->setFontName("SimHei");//STSong  //Microsoft YaHei //SimHei
    ele->setFontSize(13);
    ele->setForegroundColor(0x000000FF);
    ele->setBackgroundColor(0xFFFFFFFF);
    win->setAlignItems(Align::Center);
    win->setJustifyContent(Justify::Center);
    win->addChild(ele);
}

void TextDemo::icon()
{
    App::loadFontFile("iconfont.ttf");

    win->setFlexDirection(FlexDirection::Row);
    win->setFlexWrap(Wrap::Wrap);
    const std::wstring arr[] = { 
        L"\ue687", L"\ue688", L"\ue686", L"\ue689", L"\ue68a", 
        L"\ue68b", L"\ue68c", L"\ue68d", L"\ue68e", L"\ue68f", 
        L"\ue690", L"\ue691", L"\ue692", L"\ue693",
    };

    for (auto& iconCode:arr)
    {
        auto ele = new Label();
        ele->setText(iconCode);
        ele->setFontName("iconfont");
        ele->setFontSize(63);
        ele->setForegroundColor(0x000000FF);
        win->addChild(ele);
    }
}

void TextDemo::textblock()
{
    //auto ele = new TextBlock();
    //ele->setText(u8R"(醉里挑灯看剑，梦回吹角连营。
    //八百里分麾下炙，五十弦翻塞外声。
    //沙场秋点兵。
    //马作的卢飞快，弓如霹雳弦惊。
    //了却君王天下事，赢得生前身后名。
    //可怜白发生！)");
    //ele->setFont("Microsoft YaHei");//STSong
    //ele->setFontSize(21);
    //ele->setBackgroundColor(0xFFFFFFFF);
    //win->setAlignItems(Align::Center);
    //win->setJustifyContent(Justify::Center);
    //win->addChild(ele);
}

void TextDemo::textarea()
{
//    auto ele = new TextArea();
//    ele->setText(u8R"(一段文本，在窗口中水平垂直居中
//醉里挑灯看剑，梦回吹角连营。
//八百里分麾下炙，五十弦翻塞外声。
//沙场ABCdefglkmt秋点兵。
//马作的卢飞快，弓如霹雳弦惊。
//了却君王天下事，赢得生前身后名。
//可怜白发生！)");
//    ele->setFont("Microsoft YaHei");//STSong
//    ele->setFontSize(21);
//    ele->setBackgroundColor(0xFFFFFFFF);
//    win->setAlignItems(Align::Center);
//    win->setJustifyContent(Justify::Center);
//    win->addChild(ele);
}

void TextDemo::start() {
    label();
    //icon();
    //textblock();
    //textarea();
    win->layout();
    win->show();
}
