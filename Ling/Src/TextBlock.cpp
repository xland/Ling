#include <yoga/Yoga.h>
#include <include/core/SkFontMgr.h>
#include <include/core/SkFontStyle.h>
#include <include/ports/SkTypeface_win.h>
#include <include/core/SkFont.h>
#include <include/core/SkPaint.h>
#include <include/core/SkCanvas.h>

#include "../Include/App.h"
#include "../Include/Label.h"
#include "../Include/WindowBase.h"
#include "../Include/TextBlock.h"

namespace Ling {

    static YGSize measure(YGNodeConstRef node, float width, YGMeasureMode widthMode, float height, YGMeasureMode heightMode)
    {
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

    TextBlock::TextBlock()
    {
        YGNodeSetContext(node, this);
        YGNodeSetMeasureFunc(node, &measure);
    }
    TextBlock::~TextBlock() {

    }

    void TextBlock::paint(SkCanvas* canvas)
    {
        //sk_sp<SkFontMgr> fontMgr = SkFontMgr_New_GDI();
        //SkFontStyle fontStyle = SkFontStyle::Normal();
        //sk_sp<SkTypeface> typeFace = fontMgr->matchFamilyStyle("Microsoft YaHei", fontStyle);
        //SkFont font(typeFace, 56);

        //SkPaint paint;
        //paint.setColor(0xFF00FFFF);
        //canvas->drawString("Hello World!", 20, 120, font, paint);
        Element::paint(canvas);
        SkPaint paint;
        paint.setAntiAlias(true);
        sk_sp<SkFontMgr> fontMgr = SkFontMgr_New_GDI();
        SkFontStyle fontStyle = SkFontStyle::Normal();
        sk_sp<SkTypeface> typeFace = fontMgr->matchFamilyStyle("Microsoft YaHei", fontStyle);
        SkFont font(typeFace, 56);

        const char* text = "Hello, Bazel world! 测试";
        size_t text_length = strlen(text);

        SkRect r;
        font.measureText(text, text_length, SkTextEncoding::kUTF8, &r);

        paint.setColor(SK_ColorRED);
        canvas->drawSimpleText(text, text_length, SkTextEncoding::kUTF8, 20, 120, font, paint);
    }

    void TextBlock::setText(const std::wstring& text)
    {
        this->text = text;
    }
}