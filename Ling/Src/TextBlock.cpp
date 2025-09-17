#include <yoga/Yoga.h>
#include <thorvg.h>

#include "../Include/Util.h"
#include "../Include/App.h"
#include "../Include/TextBlock.h"
#include "../Include/WindowBase.h"

namespace Ling {

    TextBlock::TextBlock()
    {
        textShape = tvg::Text::gen();
        //YGNodeSetContext(node, this);
        //YGNodeSetMeasureFunc(node, &TextBlock::nodeMeasureCB);
    }
    TextBlock::~TextBlock() {

    }

    void TextBlock::layout()
    {
        Element::layout();
        textShape->translate(getGlobalX(), getGlobalY());
    }

    void TextBlock::setWindow(WindowBase* win)
    {
        Element::setWindow(win);
        win->scene->push(textShape);
    }


    YGSize TextBlock::nodeMeasureCB(YGNodeConstRef node, float width, YGMeasureMode widthMode, float height, YGMeasureMode heightMode)
    {
        auto tb = static_cast<TextBlock*>(YGNodeGetContext(node));
        tb->textShape->translate(0, 0);
        float x, y, w, h;
        tb->textShape->bounds(&x, &y, &w, &h);
        auto dpi = tb->getWindow()->getScaleFactor();
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
    const std::wstring& TextBlock::getText()
    {
        return text;
    }

    void TextBlock::setText(const std::wstring& text)
    {
        this->text = text;
        auto str = Ling::ConvertToUTF8(text);
        textShape->text(str.c_str());
    }

    void TextBlock::setForegroundColor(const Color& foregroundColor)
    {
        this->foregroundColor = foregroundColor;
        textShape->fill(foregroundColor.getR(), foregroundColor.getG(), foregroundColor.getB());
    }

    void TextBlock::setSize(const float& w, const float& h)
    {
        Node::setSize(w, h);
        textShape->layout(w, h);
        textShape->align(0.f, 1.f);
    }

    Color TextBlock::getForegroundColor()
    {
        return foregroundColor;
    }
    void TextBlock::setFontSize(const float& fontSize)
    {
        this->fontSize = fontSize;
        auto dpi = GetDpiForSystem() / 96.f;
        textShape->size(fontSize * dpi);
    }

    float TextBlock::getFontSize()
    {
        return fontSize;
    }

    void TextBlock::setFontName(const std::string& fontName)
    {
        this->fontName = fontName;
        textShape->font(fontName.data());
    }
    std::string TextBlock::getFontName()
    {
        return fontName;
    }
    void TextBlock::setItalic(const bool& italic)
    {
        if (italic) {
            textShape->italic();
        }
        else
        {
            textShape->italic(0);
        }
    }
    bool TextBlock::getItalic()
    {
        return italic;
    }
}
