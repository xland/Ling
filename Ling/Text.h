#pragma once
#include "Header.h"
#include "Element.h"
namespace Ling{
class Text : public Element
{
public:
	Text();
	~Text();
	void setText(const std::wstring text);
protected:
	void initProperty(const Composition::Compositor& comp) override;
private:
	ComPtr<IDWriteTextLayout> textLayout;
	std::wstring text;
};
}

