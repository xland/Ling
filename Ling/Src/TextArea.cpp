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
#include "../Include/Position.h"
#include "../Include/TextArea.h"
#include "LineGlyphInfo.h"

namespace Ling {


    TextArea::TextArea()
    {
        measuredRect = std::make_unique<SkRect>();
        YGNodeSetContext(node, this);
        YGNodeSetMeasureFunc(node, &TextArea::nodeMeasureCB);
    }
    TextArea::~TextArea() {

    }

    void TextArea::paint(SkCanvas* canvas)
    {
        Element::paint(canvas);
        float x = getLeft();
        float y = getTop();
        measure();
        SkPaint paint;
        paint.setAntiAlias(true);
        paint.setColor(SK_ColorRED);
        font->setSize(fontSize * getWindow()->scaleFactor);
        canvas->drawSimpleText(text.data(), text.length(), SkTextEncoding::kUTF8, x - measuredRect->fLeft, y - measuredRect->fTop, *font.get(), paint);
    }
    YGSize TextArea::nodeMeasureCB(YGNodeConstRef node, float width, YGMeasureMode widthMode, float height, YGMeasureMode heightMode)
    {
        auto ta = static_cast<TextArea*>(YGNodeGetContext(node));
        ta->measure();
        float measuredWidth = ta->measuredRect->width();
        float measuredHeight = ta->measuredRect->height();
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

    void TextArea::measure()
    {
        if (measuredRect->isEmpty()) {
            auto win = getWindow();
            auto sf = win->getScaleFactor();
            auto fs = fontSize * sf;
            font->setSize(fs);
            font->measureText(text.data(), text.length(), SkTextEncoding::kUTF8, measuredRect.get());
        }
    }
    const std::string& TextArea::getText()
    {
        return text;
    }

    void TextArea::setText(const std::string& text)
    {
        lineGlyphInfos.clear();
        std::stringstream ss(text);
        std::string line;
        int lineIndex{ 0 };
        SkFontMetrics metrics;
        font->getMetrics(&metrics);
        auto lineHeight = metrics.fBottom - metrics.fTop;
        size_t startIndex{0};
        while (std::getline(ss, line)) {
            LineGlyphInfo lineGlyphInfo;
            lineGlyphInfo.pos.x = getLeft();
            lineGlyphInfo.pos.y = lineIndex * lineHeight * lineSpace + getTop();
            lineGlyphInfo.glyphs.resize(line.size());

            auto glyphCount = font->textToGlyphs(line.data(), line.length(), SkTextEncoding::kUTF8, { lineGlyphInfo.glyphs.data(), line.size() });
            //lineGlyphInfos
        }

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
		startFlash();
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
