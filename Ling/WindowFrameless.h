#pragma once
#include <Windows.h>
#include <windowsx.h>
#include "WindowBase.h"
class WindowFrameless :public WindowBase
{
public:
	WindowFrameless();
	~WindowFrameless();
	void createNativeWindow() override;
public:
protected:
	LRESULT CALLBACK customMsgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) override;
private:
	LRESULT hitTest(int x, int y);
};

