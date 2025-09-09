#pragma once

#include "../Ling/Include/WindowBase.h"
using namespace Ling;
class IrregularWindow
{
	public:
		IrregularWindow();
		~IrregularWindow();
		void start();
	private:
		std::unique_ptr<WindowBase> win;
};

