#include "pch.h"
#include "../include/Color.h"

namespace Ling {

	Color::Color()
	{
	}
	Color::Color(const std::uint8_t& r, const std::uint8_t& g, const std::uint8_t& b, const std::uint8_t& a) : r{ r }, g{ g }, b{ b }, a{ a }
	{
	}
	Color::Color(const uint32_t& rgba)
	{
		r = (rgba >> 24) & 0xFF;
		g = (rgba >> 16) & 0xFF;
		b = (rgba >> 8) & 0xFF;
		a = rgba & 0xFF;
	}
	Color::Color(const std::string& rgbaStr)
	{
		uint32_t rgba = static_cast<uint32_t>(std::stoul(rgbaStr, nullptr, 16));
		r = (rgba >> 24) & 0xFF; // 右移 24 位，得到 Red
		g = (rgba >> 16) & 0xFF; // 右移 16 位，得到 Green
		b = (rgba >> 8) & 0xFF; // 右移  8 位，得到 Blue
		a = rgba & 0xFF; // 右移  0 位，得到 Alpha (最后两位)
	}
	Color::Color(const std::wstring& rgbaStr)
	{
		unsigned long rgba = std::stoul(rgbaStr, nullptr, 16);
		r = (rgba >> 24) & 0xFF;
		g = (rgba >> 16) & 0xFF;
		b = (rgba >> 8) & 0xFF;
		a = rgba & 0xFF;
	}
	Color::~Color()
	{
	}
	bool Color::isTransparent() const
	{
		return a == 0;
	}

	bool Color::operator==(const Color& other) const
	{
		return (r == other.r && g == other.g && b == other.b && a == other.a);
	}

	void Color::setRGBA(const int& r, const int& g, const int& b, const int& a)
	{
		this->r = r;
		this->b = b;
		this->g = g;
		this->a = a;
	}

	winrt::Windows::UI::Color Color::getUIColor() const
	{
		return winrt::Windows::UI::ColorHelper::FromArgb(a, r, g, b);
	}

	D2D1_COLOR_F Color::getD2DColor() const
	{
		return D2D1::ColorF(r / 255.f, g / 255.f, b / 255.f, a / 255.f);
	}
}