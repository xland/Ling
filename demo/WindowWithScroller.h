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
	void onMove(void* e);
	void onDown(void* e);
private:
	Ling::NodeScroller* scrollerBox;
	Ling::Node* titleBox;
	std::vector<Ling::Node*> btns;
	size_t onMoveId, onDownId;
	int hoverTitleBtnIndex{ -1 };
};

