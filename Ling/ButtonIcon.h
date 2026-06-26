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
		void setIconColor(const Color& color);    // 设置图标颜色
		void setIconFontSize(const float& fontSize);
	protected:
		void initProperty() override;
	private:
		std::wstring code;
	private:
		Text* iconText;
		Color iconColorFore;
		Color iconColorHover;
		Color bgColorNormal;
		Color bgColorHover;
	};
}

