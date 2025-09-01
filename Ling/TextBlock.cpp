#include <include/core/SkFontMgr.h>
#include <include/core/SkFontStyle.h>
#include <include/ports/SkTypeface_win.h>
#include <include/core/SkFont.h>
#include <include/core/SkPaint.h>
#include <include/core/SkCanvas.h>

#include "TextBlock.h"

TextBlock::TextBlock()
{

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