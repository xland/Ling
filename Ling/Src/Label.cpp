#include <yoga/Yoga.h>
#include <thorvg.h>

#include "../Include/App.h"
#include "../Include/Label.h"
#include "../Include/WindowBase.h"

namespace Ling {

    Label::Label()
    {
        textShape = tvg::Text::gen();
        YGNodeSetContext(node, this);
        YGNodeSetMeasureFunc(node, &Label::nodeMeasureCB);
    }
    Label::~Label() {

    }

    void Label::layout()
    {
        Element::layout();
        textShape->translate(getGlobalX(), getGlobalY());
    }

    void Label::setWindow(WindowBase* win)
    {
        Element::setWindow(win);
        win->scene->push(textShape);
    }


    YGSize Label::nodeMeasureCB(YGNodeConstRef node, float width, YGMeasureMode widthMode, float height, YGMeasureMode heightMode)
    {
        auto label = static_cast<Label*>(YGNodeGetContext(node));
        label->textShape->translate(0, 0);
        float x, y, w, h;
        label->textShape->bounds(&x, &y, &w, &h);
        auto dpi = label->getWindow()->getScaleFactor();
        float measuredWidth = (w+x) / dpi;
        float measuredHeight = (h+y) / dpi;
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
    const std::string& Label::getText()
    {
        return text;
    }

    void Label::setText(const std::string& text)
    {    
        this->text = text;
        textShape->text(text.c_str());
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
    void Label::setFontSize(const float& fontSize)
    {
        this->fontSize = fontSize;
        auto dpi = GetDpiForSystem() / 96.f;
        textShape->size(fontSize*dpi);
    }

    float Label::getFontSize()
    {
        return fontSize;
    }

    void Label::setFontName(const std::string& fontName)
    {
        this->fontName = fontName;
		textShape->font(fontName.data());
    }
    std::string Label::getFontName()
    {
        return fontName;
    }
    void Label::setItalic(const bool& italic)
    {
        if (italic) {
            textShape->italic();
        }
        else
        {
            textShape->italic(0);
        }
    }
	bool Label::getItalic()
    {
        return italic;
	}
}
