#pragma once
#include "../Ling/Include/WindowBase.h"
using namespace Ling;

class EventDemo
{
public:
	EventDemo();
	~EventDemo();
	void start();
private:
	std::unique_ptr<WindowBase> win;
};

