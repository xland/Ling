#pragma once
#include <string>

#include "FontWeight.h"
#include "FontWidth.h"
#include "FontSlant.h"
#include "Element.h"

class SkFont;
class Label :public Element
{
public:
	Label();
	~Label();
	void paint(SkCanvas* canvas) override;
	const std::string& getText();
	SkFont* getFont();
	void setText(const std::string& text);
	void setFont(const std::string& fontName = "Microsoft YaHei",
		const FontWeight& fontWeight = FontWeight::Normal,
		const FontWidth& fontWidth = FontWidth::Normal,
		const FontSlant& fontSlant = FontSlant::Upright);
	void setFontSize(const float& fontSize);
	float getFontSize();
protected:
	
private:
	std::string text;
	std::shared_ptr<SkFont> font;
	float fontSize{16.f};
};

