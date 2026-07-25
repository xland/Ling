#pragma once

#include "../include/Ling.h"

class TitleBar
{
public:
	TitleBar(Ling::WinBase* win);
	~TitleBar();
	LRESULT hitCaption(const POINT pt);
public:
	float height{ 30.f };
protected:
private:
	void onDown(void* e);
private:
	Ling::WinBase* win;
	Ling::Node* titleBox;
	std::vector<Ling::Button*> btns;
};

