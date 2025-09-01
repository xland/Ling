#pragma once
#include <WindowBase.h>

class EventDemo
{
public:
	EventDemo();
	~EventDemo();
	void start();
private:
	std::unique_ptr<WindowBase> win;
};

