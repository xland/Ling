import Ling;
#include "Demo.h"
#include "Ling/WindowBase.h"
Demo::Demo()
{
	x = 100;
	y = 100;
	w = 800;
	h = 600;
	createWindow(0, WS_OVERLAPPEDWINDOW);
	show();
}

Demo::~Demo()
{
}

