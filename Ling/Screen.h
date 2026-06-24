#pragma once
#include "Header.h"
namespace Ling {
	class Screen
	{
	public:
		~Screen();
		static std::tuple<int, int, int, int> getMainScreenPos();
	private:
		Screen();
	};
}

