#pragma once
#include <Windows.h>
#include <windowsx.h>
#include <string>
#include <memory>
#include "Size.h"
#include "ElementBox.h"

#define FlashCaretTimer WM_APP+1

class WindowBaseImpl;
class TextArea;
class Label;
class WindowBase:public ElementBox
{
public:
	WindowBase();
	~WindowBase();
	void show();
	void setTitle(const std::wstring& title);
	const std::wstring& getTitle();
	virtual void createNativeWindow() = 0;
	void insertChild(const int& index, Element* ele) override;
	void addChild(Element* ele) override;
	const Position& getWindowPosition();
	const Size& getWindowSize();
	const Size getWindowClientSize();
	void setWindowSize(const int& w, const int& h);
	void setWindowPosition(const int& x, const int& y);
	void setWindowToScreenCenter();

	void resetWindowSize(const int& w, const int& h);
	void resetWindowPosition(const int& x, const int& y);
	void resetWindowToScreenCenter();
	Element* getElementByPosition(int x, int y);
	HWND getHandle();
	float getScaleFactor();
public:
	friend class TextArea;
	friend class WindowBaseImpl;
	friend class Label;
protected:
	const std::wstring& getWinClsName();
	void setScaleFactor();
	virtual LRESULT CALLBACK customMsgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
protected:
	HWND hwnd;
	std::unique_ptr<WindowBaseImpl> winImpl;
	std::wstring title;
	bool resizable{ true };
private:
	static LRESULT CALLBACK routeWindowMsg(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
	LRESULT CALLBACK windowMsgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
	void windowMouseMove(const int& x, const int& y);
	void windowMouseDown(const int& x, const int& y,const MouseButton& mouseBtn);
	void windowMouseUp(const int& x, const int& y, const MouseButton& mouseBtn);
	void paintArea();
	void casecadeSetWindow(Element* ele);
private:
	/// <summary>
	/// 窗口在屏幕上的位置
	/// </summary>
	Position winPosition;
	/// <summary>
	/// 窗口的大小，
	/// 无边框窗口大小与Element大小一致
	/// 有边框窗口大小与Element大小不一致，因为有边框窗口大小包含边框和标题栏
	/// </summary>
	Size winSize;
	Element* hoverEle{ nullptr };
	Element* focusEle{ nullptr };
	float scaleFactor{ 1.f };
};

