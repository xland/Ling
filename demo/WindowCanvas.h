#pragma once
#include "include/Ling.h"
#include "TitleBar.h"
class WindowCanvas : public Ling::WinBase
{
public:
	WindowCanvas();
	~WindowCanvas();
private:
	void onCreated() override;
	LRESULT onHitTest(const POINT pos) override;
	void redraw();      // 在 canvas 中央画一个椭圆
private:
	std::unique_ptr<TitleBar> titleBar;
	Ling::Canvas* canvas{ nullptr };
};
