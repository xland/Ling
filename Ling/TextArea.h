#pragma once

#include "Element.h"

class TextArea :public Element
{
public:
	TextArea();
	~TextArea();
	void paint(SkCanvas* canvas) override;
	void setText(const std::wstring& text);
protected:
	void shown() override;
private:
	void startFlash();
	std::wstring text;
};

