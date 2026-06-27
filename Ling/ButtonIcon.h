#pragma once
#include "Element.h"
#include "Text.h"
namespace Ling{
	class WindowBase;
	class ButtonIcon : public Element<ButtonIcon>
	{
	public:
		ButtonIcon(WindowBase* win);
		~ButtonIcon();
		ButtonIcon* setIcon(const std::wstring& code);  // 设置图标编码
		ButtonIcon* setcolorFore(const Color& color);
		ButtonIcon* setcolorHoverFore(const Color& color);
		ButtonIcon* setcolorBackground(const Color& color);
		ButtonIcon* setcolorHoverBackground(const Color& color);
		ButtonIcon* setFontSize(const float& fontSize);
	protected:
		void initProperty() override;
	private:
		std::wstring code;
	private:
		Text* iconText;
		Color colorFore;
		Color colorHoverFore;
		Color colorHoverBackground;
		float fontSize{ 12.f };
	};
}

