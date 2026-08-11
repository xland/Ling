#pragma once
#include "include/Ling.h"
#include "TitleBar.h"
class WindowTextBox : public Ling::WinBase
{
public:
	WindowTextBox();
	~WindowTextBox();
private:
	void onCreated() override;
	LRESULT onHitTest(const POINT pos) override;
	// 把字符数统计刷到 infoLabel 上
	void updateInfo();
private:
	std::unique_ptr<TitleBar> titleBar;
	Ling::TextBox* editor{ nullptr };
	Ling::TextBox* noteBox{ nullptr };
	Ling::Label* infoLabel{ nullptr };
};
