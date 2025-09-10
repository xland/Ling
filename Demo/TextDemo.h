#pragma once
#include "../Ling/Include/WindowBase.h"
using namespace Ling;
class TextDemo
{
public:
	TextDemo();
	~TextDemo();
	void label();
	void icon();
	void textblock();
	void textarea();
	void start();
private:
	std::unique_ptr<WindowBase> win;
};

