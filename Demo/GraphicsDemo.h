#pragma once
#include "../Ling/Include/WindowBase.h"
using namespace Ling;
class GraphicsDemo
{
	public:
		GraphicsDemo();
		~GraphicsDemo();
		void start();
	private:
		std::unique_ptr<WindowBase> win;
};

