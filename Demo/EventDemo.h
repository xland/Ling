#pragma once
#include "../Ling/Include/WindowBase.h"
using namespace Ling;

class EventDemo
{
public:
	EventDemo();
	~EventDemo();
	void enterLeaveDownUp();
	void counter();
	void start();
private:
	std::unique_ptr<WindowBase> win;
};

