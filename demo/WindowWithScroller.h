#pragma once
#include "include/Ling.h"
#include "TitleBar.h"
class WindowWithScroller : public Ling::WinBase
{
public:
	WindowWithScroller();
	~WindowWithScroller();
private:
	void onCreated() override;
	LRESULT onHitTest(const POINT pos) override;
	void initScrollerBox();
private:
	Ling::ScrollerBox* scrollerBox;
	std::unique_ptr<TitleBar> titleBar;
};

