#include <sstream>
#include <yoga/Yoga.h>
#include <include/core/SkFontMgr.h>
#include <include/core/SkFontMetrics.h>
#include <include/core/SkFontStyle.h>
#include <include/ports/SkTypeface_win.h>
#include <include/core/SkFont.h>
#include <include/core/SkPaint.h>
#include <include/core/SkCanvas.h>


#include <include/core/SkTextBlob.h>

#include "../Include/App.h"
#include "../Include/WindowBase.h"
#include "../Include/Position.h"
#include "../Include/TextArea.h"
#include "LineGlyphInfo.h"

namespace Ling {
    TextArea::TextArea()
    {
        YGNodeSetContext(node, this);
        YGNodeSetMeasureFunc(node, &TextArea::nodeMeasureCB);
    }
    TextArea::~TextArea() {

    }

    void TextArea::paint(SkCanvas* canvas)
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
        for (auto& info : lineGlyphInfos)
        {
            canvas->drawGlyphs({ info.glyphs.data(),info.glyphs.size() }, 
                { info.wordPos.data() ,info.wordPos.size()}, SkPoint(x, y), *font.get(), paint);
        }
    }
    YGSize TextArea::nodeMeasureCB(YGNodeConstRef node, float width, YGMeasureMode widthMode, float height, YGMeasureMode heightMode)
    {
        auto ta = static_cast<TextArea*>(YGNodeGetContext(node));
        ta->measure();
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

    void TextArea::measure()
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
    }
    const std::string& TextArea::getText()
    {
        return text;
    }

    void TextArea::setText(const std::string& text)
    {
        this->text = text;
    }

    void TextArea::setFont(const std::string& fontName, const FontWeight& fontWeight, const FontWidth& fontWidth, const FontSlant& fontSlant)
    {
        SkFontStyle fontStyle((SkFontStyle::Weight)fontWeight, (SkFontStyle::Width)fontWidth, (SkFontStyle::Slant)fontSlant);
        sk_sp<SkTypeface> typeFace = App::getFontMgr()->matchFamilyStyle(fontName.data(), fontStyle);
        font = std::make_shared<SkFont>(typeFace);
        font->setEdging(SkFont::Edging::kSubpixelAntiAlias);
        font->setSubpixel(true);
    }

    void TextArea::setFontSize(const float& fontSize)
    {
        this->fontSize = fontSize;
    }

    float TextArea::getFontSize()
    {
        return fontSize;
    }

	void TextArea::shown()
	{
		Element::shown();
		//startFlash();
	}

	void TextArea::startFlash()
	{
		auto win = getWindow();
		win->focusEle = this;
		auto hwnd = win->getHandle();
		SetTimer(hwnd, FlashCaretTimer, 600, NULL); //每600毫秒触发一次
		SendMessage(hwnd, WM_TIMER, FlashCaretTimer, 0); //马上触发一次
	}
}
