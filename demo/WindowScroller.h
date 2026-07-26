#pragma once
#include "include/Ling.h"
#include "TitleBar.h"
class WindowScroller : public Ling::WinBase
{
public:
	WindowScroller();
	~WindowScroller();
private:
	void onCreated() override;
	LRESULT onHitTest(const POINT pos) override;
	void initScrollerBox();
private:
	Ling::ScrollerBox* scrollerBox;
	std::unique_ptr<TitleBar> titleBar;
};

