#pragma once
#include <WindowBase.h>

class InputDemo
{
public:
	InputDemo();
	~InputDemo();
	void start();
private:
	std::unique_ptr<WindowBase> win;
};
