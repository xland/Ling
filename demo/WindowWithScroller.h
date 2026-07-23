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
	void initScrollerBox();
private:
	std::unique_ptr<Ling::NodeScroller> scrollerBox;
	std::unique_ptr<Ling::Node> titleBar;
};

