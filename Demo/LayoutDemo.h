#pragma once

#include "../Ling/Include/WindowBase.h"
using namespace Ling;
class LayoutDemo
{
public:
	LayoutDemo();
	~LayoutDemo();
	void verticalHorizontalCenter();
	void leftRight();
	void topCenterBottom();
	void start();
private:
	std::unique_ptr<WindowBase> win;
};

