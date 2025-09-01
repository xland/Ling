#pragma once
#include <Windows.h>
#include <windowsx.h>
#include <string>
#include <vector>

#include "WindowBase.h"

class WindowNormal:public WindowBase
{
public:
	void createNativeWindow() override;
protected:
private:
	//LRESULT hitTest(int x, int y);
	//void createWindow();
	//int x, y, width, height;
	//bool frame, shadow , visible,center, resizable, transition;
	//std::wstring title;
	//static std::string className;
};

