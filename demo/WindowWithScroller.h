#pragma once
#include "include/WinBase.h"
#include "include/NodeScroller.h"
#include "include/NodeText.h"
class WindowWithScroller : public Ling::WinBase
{
public:
	WindowWithScroller();
	~WindowWithScroller();
private:
	void onCreated() override;
	LRESULT onHitTest(const POINT& pos) override;
	void initScrollerBox();
private:
	Ling::NodeScroller* scrollerBox;
	Ling::Node* titleBox;
};

