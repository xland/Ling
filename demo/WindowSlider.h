#pragma once
#include "include/Ling.h"
#include "TitleBar.h"
class WindowSlider : public Ling::WinBase
{
public:
	WindowSlider();
	~WindowSlider();
private:
	void onCreated() override;
	LRESULT onHitTest(const POINT pos) override;
private:
	std::unique_ptr<TitleBar> titleBar;
	Ling::Slider* slider{ nullptr };
	Ling::Slider* stepSlider{ nullptr };
	Ling::Label* valueLabel{ nullptr };
	Ling::Label* stepLabel{ nullptr };
};
