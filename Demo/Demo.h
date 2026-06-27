#pragma once
#include "Ling/WindowBase.h"
class Demo : public Ling::WindowBase
{
public:
	Demo();
	~Demo();
private:
	LRESULT onHitTest(const int& x, const int& y) override;
	Ling::Box* titleBox;
};

