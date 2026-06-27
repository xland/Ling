#pragma once
#include <tuple>
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

