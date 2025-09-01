#pragma once
#include <cstdint>

typedef uint32_t Color;
int getColorA(const Color& color);
int getColorB(const Color& color);
int getColorG(const Color& color);
int getColorR(const Color& color);
Color toColor(const int& r = 0, const int& g = 0, const int& b = 0, const int& a = 255);

constexpr Color colorTransparent = 0xFF000000;
constexpr Color colorBlack = 0xFF000000;
constexpr Color colorWhite = 0xFFFFFFFF;
constexpr Color colorRed = 0xFFFF0000;
constexpr Color colorGreen = 0xFF008000;
constexpr Color colorBlue = 0xFF0000FF;
constexpr Color colorYellow = 0xFFFFFF00;
constexpr Color colorCyan = 0xFF00FFFF;
constexpr Color colorGlod = 0xFFFFD700;
constexpr Color colorGray = 0xFF808080;
constexpr Color colorLightGray = 0xFFD3D3D3;
constexpr Color colorDarkGray = 0xFFA9A9A9;
constexpr Color colorOrange = 0xFFFFA500;
constexpr Color colorPurple = 0xFF800080;
constexpr Color colorPink = 0xFFFFC0CB;
constexpr Color colorBrown = 0xFFA52A2A;
constexpr Color colorDeepPink = 0xFFFF1493;
constexpr Color colorNavy = 0xFF000080;
constexpr Color colorOlive = 0xFF808000;
constexpr Color colorTeal = 0xFF008080;
constexpr Color colorSilver = 0xFFC0C0C0;
constexpr Color colorAqua = 0xFF00FFFF;

