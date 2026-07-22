#pragma once
#include "include/WinBase.h"
class WindowWithScroller : public Ling::WinBase
{
public:
	WindowWithScroller();
	~WindowWithScroller();
private:
	void onCreated() override;
};

