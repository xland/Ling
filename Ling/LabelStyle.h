#pragma once
#include "ElementStyle.h"

namespace Ling {
	class LabelStyle : public ElementStyle
	{
		friend class Label;
	public:
		LabelStyle();
		virtual ~LabelStyle();
		void setForegroundColor(const Color& color);
		void setForegroundHoverColor(const Color& color);
		void setFontSize(const float& fontSize);
		void setFontFamily(const std::wstring& fontFamily);
		const std::wstring& getFontFamily();
		const float& getFontSize();
		const Color& getForegroundColor();
		const Color& getForegroundHoverColor();
	private:
		Color foregroundColor{ 0x00000000 };
		Color foregroundHoverColor{ 0x00000000 };
		std::wstring fontFamily{L"Microsoft YaHei"};
		float fontSize{ 14.f };
	};
}

