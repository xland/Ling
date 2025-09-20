#include "ImageDemo.h"

#include "../Ling/Include/WindowFrameless.h"
#include "../Ling/Include/WindowNormal.h"
#include "../Ling/Include/Image.h"

ImageDemo::ImageDemo()
{
    //win = std::make_unique<WindowFrameless>();
    win = std::make_unique<WindowNormal>();
    win->setTitle(L"Ling 示例窗口");
    win->setWindowSize(1000, 800);
    win->setWindowToScreenCenter();
    win->setBackgroundColor(0x234567FF);
    win->createNativeWindow();
}
ImageDemo::~ImageDemo()
{

}

void ImageDemo::png()
{
    auto ele = std::make_shared<Image>();
    ele->setSize(200, 200);
    ele->setSrc(L"img.png");
    win->setAlignItems(Align::Center);
    win->setJustifyContent(Justify::Center);
    win->addChild(ele);
}

void ImageDemo::jpg()
{
    auto ele = std::make_shared<Image>();
    ele->setSrc(L"img.jpg");
    win->setAlignItems(Align::Center);
    win->setJustifyContent(Justify::Center);
    win->addChild(ele);
}

void ImageDemo::webp()
{
    auto ele = std::make_shared<Image>();
    ele->setSrc(L"img.webp");
    win->setAlignItems(Align::Center);
    win->setJustifyContent(Justify::Center);
    win->addChild(ele);
}

void ImageDemo::svg()
{
    auto ele = std::make_shared<Image>();
    ele->setSrc(L"img.svg");
    ele->setSize(260, 260);
    win->setAlignItems(Align::Center);
    win->setJustifyContent(Justify::Center);
    win->addChild(ele);
}

void ImageDemo::start()
{
    //png();
    //jpg();
    //webp();
    svg();
    win->layout();
    win->show();
}

