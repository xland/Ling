#pragma once
#include "include/Ling.h"
class WindowWithScroller : public Ling::WinBase
{
public:
	WindowWithScroller();
	~WindowWithScroller();
private:
	void onCreated() override;
	LRESULT onHitTest(const POINT& pos) override;
	void initTitleBar();
	void initScrollerBox();
	void onMove(void* e);
	void onDown(void* e);
private:
	Ling::NodeScroller* scrollerBox;
	Ling::Node* titleBox;
	std::vector<Ling::Button*> btns;
	size_t onMoveId, onDownId;
	int hoverTitleBtnIndex{ -1 };
};

