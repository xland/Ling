#pragma once
#include "WindowBase.h"
class Demo : public WindowBase
{
public:
	Demo();
	~Demo();
private:
	void onPaint() override;
};

