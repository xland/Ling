#pragma once
#include "Element.h"
#include "Text.h"
namespace Ling{
	class ButtonIcon : public Element
	{
	public:
		ButtonIcon(WindowBase* win);
		~ButtonIcon();
		void setIcon(const std::wstring& code);  // 设置图标编码
		void setcolorFore(const Color& color);
		void setcolorHoverFore(const Color& color);
		void setcolorBackground(const Color& color);
		void setcolorHoverBackground(const Color& color);
		void setFontSize(const float& fontSize);
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

