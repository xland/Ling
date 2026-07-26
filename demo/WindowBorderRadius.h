#pragma once
#include "include/Ling.h"
#include "TitleBar.h"
class WindowBorderRadius : public Ling::WinBase
{
public:
	WindowBorderRadius();
	~WindowBorderRadius();
private:
	void onCreated() override;
	LRESULT onHitTest(const POINT pos) override;
private:
	std::unique_ptr<TitleBar> titleBar;
	Ling::Button* button{ nullptr };
};
