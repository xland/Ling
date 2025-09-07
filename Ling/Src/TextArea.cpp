#include <sstream>
#include <format>
#include <yoga/Yoga.h>
#include <thorvg.h>

#include "../Include/App.h"
#include "../Include/WindowBase.h"
#include "../Include/Position.h"
#include "../Include/TextArea.h"

namespace Ling {
    TextArea::TextArea()
    {
        YGNodeSetContext(node, this);
        YGNodeSetMeasureFunc(node, &TextArea::nodeMeasureCB);
        setCursor(Cursor::Ibeam);
    }
    TextArea::~TextArea() {

    }

    //void TextArea::paint(SkCanvas* canvas)
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
    //    for (auto& info : lineGlyphInfos)
    //    {
    //        canvas->drawGlyphs({ info.glyphs.data(),info.glyphs.size() }, 
    //            { info.wordPos.data() ,info.wordPos.size()}, SkPoint(x, y), *font.get(), paint);
    //        canvas->drawLine(SkPoint::Make(x, info.wordPos[0].fY+y), SkPoint::Make(measuredWidth+x, info.wordPos[0].fY+y), paint);
    //    }
    //}
    YGSize TextArea::nodeMeasureCB(YGNodeConstRef node, float width, YGMeasureMode widthMode, float height, YGMeasureMode heightMode)
    {
        auto ta = static_cast<TextArea*>(YGNodeGetContext(node));
        //ta->measure();
        float mWidth = ta->measuredWidth;
        float mHeight = ta->measuredHeight;
        if (widthMode == YGMeasureModeExactly) {
            mWidth = width;
        }
        else if (widthMode == YGMeasureModeAtMost) {
            mWidth = std::min(mWidth, width);
        }
        if (heightMode == YGMeasureModeExactly) {
            mHeight = height;
        }
        else if (heightMode == YGMeasureModeAtMost) {
            mHeight = std::min(mHeight, height);
        }
        return { mWidth, mHeight };
    }

    /*void TextArea::measure()
    {
        if (!lineGlyphInfos.empty()) return;
        std::stringstream ss(text);
        
        std::string line;
        {
            auto win = getWindow();
            auto sf = win->getScaleFactor();
            auto fs = fontSize * sf;
            font->setSize(fs);
        }
        float ascent, descent,lineHeight;
        {
            SkFontMetrics metrics;
            font->getMetrics(&metrics);
            ascent = metrics.fAscent;
            descent = metrics.fDescent;
            lineHeight = metrics.fBottom - metrics.fTop;
        }
        measuredHeight = 0.f;
        measuredWidth = 0.f;
        while (std::getline(ss, line)) {
            LineGlyphInfo info;
            int textCount = font->countText(line.data(), line.length(), SkTextEncoding::kUTF8);
            info.glyphs.resize(textCount);
            auto glyphCount = font->textToGlyphs(line.data(),  line.length(), 
                SkTextEncoding::kUTF8, {info.glyphs.data(), textCount});
            std::vector<SkScalar> widths;
            widths.resize(glyphCount);
            font->getWidths({ info.glyphs.data(), glyphCount }, { widths.data() ,glyphCount });
            info.wordPos.resize(glyphCount + 1);
            measuredHeight += lineSpace / 2;
            float x = 0;
            float y = measuredHeight - ascent;
            for (int i = 0; i < glyphCount; ++i) {
                info.wordPos[i] = SkPoint::Make(x, y);
                x += widths[i];
            }
            info.wordPos[glyphCount] = SkPoint::Make(x, y);
            if (measuredWidth < x) measuredWidth = x;
            measuredHeight += lineHeight;
            lineGlyphInfos.push_back(info);
        }
        measuredHeight += lineSpace / 2 - descent;
    }*/

    const std::string& TextArea::getText()
    {
        return text;
    }

    void TextArea::setText(const std::string& text)
    {
        this->text = text;
    }

    //void TextArea::setFont(const std::string& fontName, const FontWeight& fontWeight, const FontWidth& fontWidth, const FontSlant& fontSlant)
    //{
    //    SkFontStyle fontStyle((SkFontStyle::Weight)fontWeight, (SkFontStyle::Width)fontWidth, (SkFontStyle::Slant)fontSlant);
    //    sk_sp<SkTypeface> typeFace = App::getFontMgr()->matchFamilyStyle(fontName.data(), fontStyle);
    //    font = std::make_shared<SkFont>(typeFace);
    //    font->setEdging(SkFont::Edging::kSubpixelAntiAlias);
    //    font->setSubpixel(true);
    //}

    void TextArea::setFontSize(const float& fontSize)
    {
        this->fontSize = fontSize;
    }

    float TextArea::getFontSize()
    {
        return fontSize;
    }

    /*void TextArea::mouseDown(const MouseEvent& event)
    {
        Event::mouseDown(event);
        caretVisible = true;
        float ascent, descent, lineHeight;
        {
            SkFontMetrics metrics;
            font->getMetrics(&metrics);
            ascent = metrics.fAscent;
            descent = metrics.fDescent;
            lineHeight = metrics.fBottom - metrics.fTop;
        }
        for (auto& info : lineGlyphInfos)
        {
            auto startY = info.wordPos[0].fY + ascent - lineSpace / 2;
            auto endY = startY + lineHeight + lineSpace;
            if (event.y >= startY && event.y < endY) {
                for (size_t i = 0; i < info.wordPos.size(); i++)
                {
                    auto& word = info.wordPos[i];
                    if (event.x > word.fX) {

                    }
                }
                break;
            }
            OutputDebugString(std::format(L"{},{}----{},{}\n", info.wordPos[0].fX, info.wordPos[0].fY,event.x,event.y).data());
        }
        auto win = getWindow();
        win->setFocusEle(this);
    }*/
    void TextArea::shown()
    {
        //Event::shown();
        //auto win = getWindow();
        //win->onDpiChanged([this]() {
        //    lineGlyphInfos.clear();
        //    YGNodeMarkDirty(node);
        //});
    }
}
