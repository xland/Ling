#include <thorvg.h>
#include "../Include/Color.h"

namespace Ling{
	Color::Color()
	{
	}
	Color::Color(const std::uint8_t& a, const std::uint8_t& r, const std::uint8_t& g, const std::uint8_t& b):
		a{ a }, r{ r }, g{ g }, b{ b }
	{

	}
	Color::Color(const uint32_t& argb)
	{
		r = (argb >> 24) & 0xFF;
		g = (argb >> 16) & 0xFF;
		b = (argb >> 8) & 0xFF;
		a = argb & 0xFF;
	}
	Color::Color(const std::string& rgbaStr)
	{
		uint32_t rgba = static_cast<uint32_t>(std::stoul(rgbaStr, nullptr, 16));
		r = (rgba >> 24) & 0xFF; // 右移 24 位，得到 Red
		g = (rgba >> 16) & 0xFF; // 右移 16 位，得到 Green
		b = (rgba >> 8) & 0xFF; // 右移  8 位，得到 Blue
		a = rgba & 0xFF; // 右移  0 位，得到 Alpha (最后两位)
	}
	Color::~Color()
	{
	}
	bool Color::isTransparent()
	{
		return a == 0;
	}
	std::uint8_t Color::getA() const
	{
		return a;
	}
	std::uint8_t Color::getR() const
	{
		return r;
	}
	std::uint8_t Color::getG() const
	{
		return g;
	}
	std::uint8_t Color::getB() const
	{
		return b;
	}

	//constexpr Color colorTransparent = 0xFF000000;
	//constexpr Color colorBlack = 0xFF000000;
	//constexpr Color colorWhite = 0xFFFFFFFF;
	//constexpr Color colorRed = 0xFFFF0000;
	//constexpr Color colorGreen = 0xFF008000;
	//constexpr Color colorBlue = 0xFF0000FF;
	//constexpr Color colorYellow = 0xFFFFFF00;
	//constexpr Color colorCyan = 0xFF00FFFF;
	//constexpr Color colorGlod = 0xFFFFD700;
	//constexpr Color colorGray = 0xFF808080;
	//constexpr Color colorLightGray = 0xFFD3D3D3;
	//constexpr Color colorDarkGray = 0xFFA9A9A9;
	//constexpr Color colorOrange = 0xFFFFA500;
	//constexpr Color colorPurple = 0xFF800080;
	//constexpr Color colorPink = 0xFFFFC0CB;
	//constexpr Color colorBrown = 0xFFA52A2A;
	//constexpr Color colorDeepPink = 0xFFFF1493;
	//constexpr Color colorNavy = 0xFF000080;
	//constexpr Color colorOlive = 0xFF808000;
	//constexpr Color colorTeal = 0xFF008080;
	//constexpr Color colorSilver = 0xFFC0C0C0;
	//constexpr Color colorAqua = 0xFF00FFFF;
}
