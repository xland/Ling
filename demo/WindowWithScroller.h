#pragma once
#include "include/WinBase.h"
#include "include/NodeScroller.h"
class WindowWithScroller : public Ling::WinBase
{
public:
	WindowWithScroller();
	~WindowWithScroller();
private:
	void onCreated() override;
	void onSize(void* arg);
private:
	Ling::NodeScroller* scrollerBox;
};

