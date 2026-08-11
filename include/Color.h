#pragma once
#include <string>
#include <cstdint>
#include <d2d1_1.h>
#include <winrt/Windows.UI.h>

namespace Ling {
	class Color
	{
	public:
		Color();
		~Color();
		Color(uint8_t r, uint8_t g, uint8_t b, uint8_t a = 255);
		Color(uint32_t rgba);
		Color(const std::string& rgbaStr);
		Color(const std::wstring& rgbaStr);
		bool operator==(const Color& other) const;
		bool isTransparent() const;
		void setRGBA(int r, int g, int b, int a = 255);
		winrt::Windows::UI::Color getUIColor() const;
		D2D1_COLOR_F getD2DColor() const;
	public:
		uint8_t a{ 0 };
		uint8_t r{ 0 };
		uint8_t g{ 0 };
		uint8_t b{ 0 };
	};
}