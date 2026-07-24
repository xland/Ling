#pragma once
#include "include/Ling.h"
#include "TitleBar.h"
class WindowImage : public Ling::WinBase
{
public:
	WindowImage();
	~WindowImage();
private:
	void onCreated() override;
	LRESULT onHitTest(const POINT pos) override;
	void onDown(void* e);
private:
	std::unique_ptr<TitleBar> titleBar;
	Ling::Label* tipLabel;
};

