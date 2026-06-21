#include "pch.h"
#include "Demo.h"

Demo::Demo()
{
	x = 100;
	y = 100;
	w = 800;
	h = 600;
	createWindow();
	show();
}

Demo::~Demo()
{
}

void Demo::onPaint()
{
}
