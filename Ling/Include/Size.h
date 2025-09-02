#pragma once
namespace Ling {
	class Size
	{
	public:
		Size();
		Size(const int& w = 0, const int& h = 0);
		~Size();
	public:
		int w, h;
	private:
	};
}

