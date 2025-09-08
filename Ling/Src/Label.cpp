#include <yoga/Yoga.h>
#include <thorvg.h>

#include "../Include/App.h"
#include "../Include/Label.h"
#include "../Include/WindowBase.h"

namespace Ling {
    Label::Label()
    {
        YGNodeSetContext(node, this);
        YGNodeSetMeasureFunc(node, &Label::nodeMeasureCB);
        textShape = tvg::Text::gen();
    }
    Label::~Label() {

    }

    void Label::layout()
    {
        Element::layout();
    }

    void Label::setWindow(WindowBase* win)
    {
        Element::setWindow(win);
        win->scene->push(textShape);
    }


    YGSize Label::nodeMeasureCB(YGNodeConstRef node, float width, YGMeasureMode widthMode, float height, YGMeasureMode heightMode)
    {
        auto label = static_cast<Label*>(YGNodeGetContext(node));
        
        //float x, y, w, h;
        //label->textShape->bounds(&x, &y, &w, &h);

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
        return { 300, 60 };
    }
    const std::u8string& Label::getText()
    {
        return text;
    }

    void Label::setText(const std::u8string& text)
    {    
        this->text = text;
        textShape->text(reinterpret_cast<const char*>(text.c_str()));
        textShape->translate(10, 10);
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
        textShape->size(fontSize);
    }

    float Label::getFontSize()
    {
        return fontSize;
    }

    void Label::setFontName(const std::string& fontName)
    {
		App::loadFont(fontName);
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
