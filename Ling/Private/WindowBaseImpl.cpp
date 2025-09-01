#include <include/core/SkCanvas.h>
#include <include/encode/SkPngEncoder.h>
#include <include/core/SkStream.h>

#include "../Element.h"
#include "../WindowBase.h"
#include "WindowBaseImpl.h"


WindowBaseImpl::WindowBaseImpl(WindowBase* win):win{win}
{
}

WindowBaseImpl::~WindowBaseImpl()
{
}

void WindowBaseImpl::paintElement(Element* ele)
{
    if (!surface) reset();
    auto canvas = surface->getCanvas(); //todo 不要每次重绘都全量绘制
    canvas->clear(0x000000);
    canvas->save();
    canvas->scale(win->scaleFactor, win->scaleFactor);
    ele->paint(canvas);
    canvas->restore();

    //SkPixmap pixmap;
    //surface->peekPixels(&pixmap);
    //SkFILEWStream stream("allen1.png");
    //SkPngEncoder::Encode(&stream, pixmap, {});
    //stream.flush();
}

const SkPixmap WindowBaseImpl::getPix()
{
    SkPixmap pix;
    surface->peekPixels(&pix);
    return pix;
}

void WindowBaseImpl::reset()
{
    surface.reset(nullptr);
    auto size = win->getWindowClientSize();
    win->setSize(size.w, size.h);
    SkImageInfo info = SkImageInfo::MakeN32Premul(size.w * win->scaleFactor, size.h * win->scaleFactor);
    surface = SkSurfaces::Raster(info);
}
