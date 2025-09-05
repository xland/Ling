#include <sstream>
#include <yoga/Yoga.h>
#include <include/core/SkFontMgr.h>
#include <include/core/SkFontMetrics.h>
#include <include/core/SkFontStyle.h>
#include <include/ports/SkTypeface_win.h>
#include <include/core/SkFont.h>
#include <include/core/SkPaint.h>
#include <include/core/SkCanvas.h>

#include "../Include/App.h"
#include "../Include/WindowBase.h"
#include "../Include/TextBlock.h"
#include "../Include/Position.h"
#include "LineSizeInfo.h"

namespace Ling {
    TextBlock::TextBlock()
    {
        YGNodeSetContext(node, this);
        YGNodeSetMeasureFunc(node, &TextBlock::nodeMeasureCB);
    }
    TextBlock::~TextBlock() {

    }

    void TextBlock::paint(SkCanvas* canvas)
    {
        Element::paint(canvas);
        if (text.empty()) return;
        measure();
        float x = getLeft();
        float y = getTop();
        SkPaint paint;
        paint.setAntiAlias(true);
        paint.setColor(SK_ColorRED);
        font->setSize(fontSize * getWindow()->scaleFactor);
        for (auto& info: lineSizeInfos)
        {
            auto str = text.substr(info.startIndex, info.length);
            canvas->drawSimpleText(str.data(), str.length(), SkTextEncoding::kUTF8, x+info.pos.x, y+info.pos.y, *font.get(), paint);
        }
        
    }
    YGSize TextBlock::nodeMeasureCB(YGNodeConstRef node, float width, YGMeasureMode widthMode, float height, YGMeasureMode heightMode)
    {
        auto tb = static_cast<TextBlock*>(YGNodeGetContext(node));
        tb->measure();
        float mWidth = tb->measuredWidth;
        float mHeight = tb->measuredHeight;
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

    const std::string& TextBlock::getText()
    {
        return text;
    }
    void TextBlock::measure()
    {
        if (!lineSizeInfos.empty()) return;
        std::stringstream ss(text);
        std::string line;
        auto win = getWindow();
        auto sf = win->getScaleFactor();
        auto fs = fontSize * sf;
        font->setSize(fs);
        SkRect rect;
        size_t startIndex{ 0 };
        while (std::getline(ss, line)) {
            font->measureText(line.data(), line.length(), SkTextEncoding::kUTF8, &rect);
            auto w = rect.width();
            auto h = rect.height();
            LineSizeInfo lineInfo;
            if (measuredWidth < w) measuredWidth = w;
            measuredHeight += lineSpace/2;
            lineInfo.pos.x = 0 - rect.fLeft;
            lineInfo.pos.y = measuredHeight - rect.fTop;
            lineInfo.startIndex = startIndex;
            lineInfo.length = line.length();
            lineSizeInfos.push_back(std::move(lineInfo));
            startIndex += lineInfo.length+1;
            measuredHeight += h;
        }
        measuredHeight += lineSpace / 2;
    }
    void TextBlock::setText(const std::string& text)
    {
        this->text = text;
    }

    void TextBlock::setFont(const std::string& fontName, const FontWeight& fontWeight, const FontWidth& fontWidth, const FontSlant& fontSlant)
    {
        SkFontStyle fontStyle((SkFontStyle::Weight)fontWeight, (SkFontStyle::Width)fontWidth, (SkFontStyle::Slant)fontSlant);
        sk_sp<SkTypeface> typeFace = App::getFontMgr()->matchFamilyStyle(fontName.data(), fontStyle);
        font = std::make_shared<SkFont>(typeFace);
        font->setEdging(SkFont::Edging::kSubpixelAntiAlias);
        font->setSubpixel(true);
    }

    void TextBlock::setFontSize(const float& fontSize)
    {
        this->fontSize = fontSize;
    }

    float TextBlock::getFontSize()
    {
        return fontSize;
    }
}