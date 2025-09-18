#include <yoga/Yoga.h>
#include <thorvg.h>
#include "../Include/Image.h"
#include "../Include/WindowBase.h"
#include "../Include/Util.h"

namespace Ling {
	Image::Image()
	{
        imgShape = tvg::Picture::gen();
        YGNodeSetContext(node, this);
        YGNodeSetMeasureFunc(node, &Image::nodeMeasureCB);
	}

	Image::~Image()
	{
	}

    YGSize Image::nodeMeasureCB(YGNodeConstRef node, float width, YGMeasureMode widthMode, float height, YGMeasureMode heightMode)
    {
        auto label = static_cast<Image*>(YGNodeGetContext(node));
        label->imgShape->translate(0, 0);
        float x, y, w, h;
        label->imgShape->bounds(&x, &y, &w, &h);
        auto dpi = label->getWindow()->getScaleFactor();
        float measuredWidth = (w + x) / dpi;
        float measuredHeight = (h + y) / dpi;
        if (widthMode == YGMeasureModeExactly) {
            measuredWidth = width;
        }
        else if (widthMode == YGMeasureModeAtMost) {
            measuredWidth = std::min(measuredWidth, width);
        }
        if (heightMode == YGMeasureModeExactly) {
            measuredHeight = height;
        }
        else if (heightMode == YGMeasureModeAtMost) {
            measuredHeight = std::min(measuredHeight, height);
        }
        return { measuredWidth, measuredHeight };
    }

    void Image::layout()
    {
        Element::layout();
        imgShape->translate(getGlobalX(), getGlobalY());
    }

    void Image::setWindow(WindowBase* win)
    {
        Element::setWindow(win);
        auto size = getSize();
        imgShape->size(size.w, size.h);
        win->scene->push(imgShape);
    }

    const std::wstring& Image::getSrc()
    {
        return src;
    }

    void Image::setSrc(const std::wstring& src)
    {
        this->src = src;
        auto str = Ling::ConvertToUTF8(src);
        imgShape->load(str.data());
    }
    void Image::setSize(const float& w, const float& h)
    {
        Node::setSize(w, h);
    }
}