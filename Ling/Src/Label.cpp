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

    void Label::layout()
    {
        Element::layout();
        textShape = tvg::Text::gen();
        tvg::Text::load("/arial.ttf");
        textShape->font("Arial");
    }

    void Label::setWindow(WindowBase* win)
    {
        Element::setWindow(win);
        win->scene->push(textShape);
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
        this->text = text;
        textShape->text(text.data());
    }

    void Label::setForegroundColor(const Color& foregroundColor)
    {
        this->foregroundColor = foregroundColor;
        textShape->fill(foregroundColor.getR(), foregroundColor.getG(), foregroundColor.getB());
    }

    Color Label::getForegroundColor()
    {
        return foregroundColor;
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
        textShape->size(fontSize);
    }

    float Label::getFontSize()
    {
        return fontSize;
    }

    void Label::setFontName(const std::string& fontName)
    {
        if (fontName == "Arial") {
            tvg::Text::load("C:\\Windows\\Fonts\\arial.ttf");
        }
        else if (fontName == "Microsoft YaHei") {
            tvg::Text::load("C:\\Windows\\Fonts\\msyh.ttc");
        }
    }
}
