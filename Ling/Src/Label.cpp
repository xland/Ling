#include <yoga/Yoga.h>
#include <thorvg.h>

#include "../Include/App.h"
#include "../Include/Label.h"
#include "../Include/WindowBase.h"

namespace Ling {
    Label::Label()
    {
        //measuredRect = std::make_unique<SkRect>();
        YGNodeSetContext(node, this);
        YGNodeSetMeasureFunc(node, &Label::nodeMeasureCB);
    }
    Label::~Label() {

    }

    //void Label::paint(SkCanvas* canvas)
    //{
    //    Element::paint(canvas);
    //    if (text.empty()) return;
    //    measure();
    //    float x = getLeft();
    //    float y = getTop();
    //    SkPaint paint;
    //    paint.setAntiAlias(true);
    //    paint.setColor(SK_ColorRED);
    //    font->setSize(fontSize * getWindow()->scaleFactor);
    //    canvas->drawSimpleText(text.data(), text.length(), SkTextEncoding::kUTF8, x - measuredRect->fLeft, y - measuredRect->fTop, *font.get(), paint);
    //}
    YGSize Label::nodeMeasureCB(YGNodeConstRef node, float width, YGMeasureMode widthMode, float height, YGMeasureMode heightMode)
    {
        auto label = static_cast<Label*>(YGNodeGetContext(node));
        label->measure();
        float measuredWidth;// = label->measuredRect->width();
        float measuredHeight;// = label->measuredRect->height();
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

/*    void Label::measure()
    {
        if (measuredRect->isEmpty()) {
            auto win = getWindow();
            auto sf = win->getScaleFactor();
            auto fs = fontSize * sf;
            font->setSize(fs);
            font->measureText(text.data(), text.length(), SkTextEncoding::kUTF8, measuredRect.get());
        }
    }*/
    const std::string& Label::getText()
    {
        return text;
    }

    void Label::setText(const std::string& text)
    {
        //如果父容器已经约束住了大小，比如 flex:1 填充满了，Yoga 也不会再问 measureFunc。
        //如果你在 YGNodeStyleSetWidth(node, 100) / YGNodeStyleSetHeight(node, 50) 里已经指定了固定大小，Yoga 就直接用这个值，不会去调用 measureFunc。        
        this->text = text;
    }

    //void Label::setFont(const std::string& fontName, const FontWeight& fontWeight, const FontWidth& fontWidth, const FontSlant& fontSlant)
    //{
    //    SkFontStyle fontStyle((SkFontStyle::Weight)fontWeight, (SkFontStyle::Width)fontWidth, (SkFontStyle::Slant)fontSlant);
    //    sk_sp<SkTypeface> typeFace = App::getFontMgr()->matchFamilyStyle(fontName.data(), fontStyle);
    //    font = std::make_shared<SkFont>(typeFace);
    //    font->setEdging(SkFont::Edging::kSubpixelAntiAlias);
    //    font->setSubpixel(true);
    //}

    void Label::setFontSize(const float& fontSize)
    {
        this->fontSize = fontSize;
    }

    float Label::getFontSize()
    {
        return fontSize;
    }
}
