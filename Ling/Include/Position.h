#pragma once
namespace Ling {
	class Position
	{
	public:
		Position();
		Position(const int& x, const int& y);
		~Position();
	public:
		int x{ 0 }, y{ 0 };
	private:
	};
}

