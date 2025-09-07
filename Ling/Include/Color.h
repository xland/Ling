#pragma once
#include <string>
#include <cstdint>

namespace Ling {
	class Color
	{
	public:
		Color();
		Color(const uint8_t& r, const uint8_t& g, const uint8_t& b, const uint8_t& a=255);
		/// <summary>
		/// ����0x000000FF �õ���ɫ
		/// ����0xFFFFFFFF �õ���ɫ
		/// ����0x00000000 �õ�͸��
		/// </summary>
		/// <param name="argb"></param>
		Color(const uint32_t& argb);
		/// <summary>
		/// ����"000000FF" �õ���ɫ
		/// ����"FFFFFFFF" �õ���ɫ
		/// ����"00000000" �õ�͸��
		/// </summary>
		/// <param name="rgbaStr"></param>
		Color(const std::string& rgbaStr);
		~Color();
		bool isTransparent();
		uint8_t getA() const;
		uint8_t getR() const;
		uint8_t getG() const;
		uint8_t getB() const;
		
	private:
		uint8_t a;
		uint8_t r;
		uint8_t g;
		uint8_t b;
	};
}

