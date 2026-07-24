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
		Color(const uint8_t& r, const uint8_t& g, const uint8_t& b, const uint8_t& a = 255);
		Color(const uint32_t& rgba);
		Color(const std::string& rgbaStr);
		Color(const std::wstring& rgbaStr);
		bool operator==(const Color& other) const;
		bool isTransparent() const;
		void setRGBA(const int& r, const int& g, const int& b, const int& a = 255);
		winrt::Windows::UI::Color getUIColor() const;
		D2D1_COLOR_F getD2DColor() const;
	public:
		uint8_t a{ 0 };
		uint8_t r{ 0 };
		uint8_t g{ 0 };
		uint8_t b{ 0 };
	};
}