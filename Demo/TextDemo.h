#pragma once
#include <WindowBase.h>

class TextDemo
{
public:
	TextDemo();
	~TextDemo();
	void label();
	void textblock();
	void start();
private:
	std::unique_ptr<WindowBase> win;
};

