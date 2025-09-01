#include <yoga/Yoga.h>
#include <include/core/SkFontMgr.h>
#include <include/core/SkFontStyle.h>
#include <include/ports/SkTypeface_win.h>
#include <include/core/SkFont.h>
#include <include/core/SkPaint.h>
#include <include/core/SkCanvas.h>

#include "App.h"
#include "Label.h"
#include "WindowBase.h"

static YGSize measure(YGNodeConstRef node, float width, YGMeasureMode widthMode, float height, YGMeasureMode heightMode) {
    auto label = static_cast<Label*>(YGNodeGetContext(node));
    auto& text = label->getText();
    auto font = label->getFont();
    auto win = label->getWindow();
    auto fs = label->getFontSize();
    auto sf = win->getScaleFactor();
    auto fontSize = fs * sf;
    font->setSize(fontSize);
    SkRect r;
    font->measureText(text.data(), text.length(), SkTextEncoding::kUTF8, &r);
    float measuredWidth = r.width();
    float measuredHeight = r.height();
    //// 根据 Yoga 的约束模式进行裁剪
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

Label::Label()
{
    YGNodeSetContext(node, this);
    YGNodeSetMeasureFunc(node, &measure);
}
Label::~Label() {

}

void Label::paint(SkCanvas* canvas)
{
    //sk_sp<SkFontMgr> fontMgr = SkFontMgr_New_GDI();
    //SkFontStyle fontStyle = SkFontStyle::Normal();
    //sk_sp<SkTypeface> typeFace = fontMgr->matchFamilyStyle("Microsoft YaHei", fontStyle);
    //SkFont font(typeFace, 56);

    //SkPaint paint;
    //paint.setColor(0xFF00FFFF);
    //canvas->drawString("Hello World!", 20, 120, font, paint);

    Element::paint(canvas);

    float x = YGNodeLayoutGetLeft(node);
    float y = YGNodeLayoutGetTop(node);

    SkRect r;
    font->measureText(text.data(), text.length(), SkTextEncoding::kUTF8, &r);

    SkPaint paint;
    paint.setAntiAlias(true);
    paint.setColor(SK_ColorRED);
    font->setSize(fontSize * win->scaleFactor);
    canvas->drawSimpleText(text.data(), text.length(), SkTextEncoding::kUTF8, x - r.fLeft, y - r.fTop, *font.get(), paint);
}

const std::string& Label::getText()
{
    return text;
}

SkFont* Label::getFont()
{
    return font.get();
}

void Label::setText(const std::string& text)
{
    //如果父容器已经约束住了大小，比如 flex:1 填充满了，Yoga 也不会再问 measureFunc。
    //如果你在 YGNodeStyleSetWidth(node, 100) / YGNodeStyleSetHeight(node, 50) 里已经指定了固定大小，Yoga 就直接用这个值，不会去调用 measureFunc。
    
    //SkRect r;
    //font.measureText(text, text_length, SkTextEncoding::kUTF8, &r);
    this->text = text;
}

void Label::setFont(const std::string& fontName, const FontWeight& fontWeight, const FontWidth& fontWidth, const FontSlant& fontSlant)
{
    SkFontStyle fontStyle((SkFontStyle::Weight)fontWeight, (SkFontStyle::Width)fontWidth, (SkFontStyle::Slant)fontSlant);
    sk_sp<SkTypeface> typeFace = App::getFontMgr()->matchFamilyStyle(fontName.data(), fontStyle);    
    font = std::make_shared<SkFont>(typeFace);
    font->setEdging(SkFont::Edging::kSubpixelAntiAlias);
    font->setSubpixel(true);
}

void Label::setFontSize(const float& fontSize)
{
    this->fontSize = fontSize;
}

float Label::getFontSize()
{
    return fontSize;
}
