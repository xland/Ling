#pragma once
#include "../Ling/Include/WindowBase.h"
using namespace Ling;
class ImageDemo
{
public:
	ImageDemo();
	~ImageDemo();
	void png();
	void jpg();
	void webp();
	void svg();
	void start();
private:
	std::unique_ptr<WindowBase> win;
};
