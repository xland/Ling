#pragma once
#include "../Ling/Include/WindowBase.h"
using namespace Ling;
class InputDemo
{
public:
	InputDemo();
	~InputDemo();
	void start();
private:
	std::unique_ptr<WindowBase> win;
};
