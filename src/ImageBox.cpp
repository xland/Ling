#include "pch.h"
#include "../include/ImageBox.h"
#include "../include/Image.h"
#include "../include/WinBase.h"

namespace Ling {

    ImageBox::ImageBox(WinBase* win) : Node(win)
    {
        // 默认让 Image 在容器内居中；使用者可通过 Node::setJustify/setAlign 覆盖。
        setJustifyContent(Justify::Center);
        setAlignItems(Align::Center);
        img = makeChild<Image>();
    }

    ImageBox::~ImageBox()
    {
    }

    void ImageBox::loadImg(const std::wstring& path) { img->loadImg(path); }
}
