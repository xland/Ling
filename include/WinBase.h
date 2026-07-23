#pragma once
#include <unordered_map>
#include <Windows.h>
#include <winrt/Windows.UI.Composition.h>
#include <winrt/Windows.UI.Composition.Desktop.h>
#include "EventBase.h"

namespace Ling {
	class Node;
	class NodeScroller;
	class WinBase:public EventBase
	{
		friend class Node;
		friend class NodeScroller;
	public:
		WinBase();
		~WinBase();
		void enableShadow();
		void show();
		void hide();
		void close();
		void minimize();
		void createNativeWindow(int iconId = 0,DWORD exStyle = NULL, DWORD style = WS_POPUP | WS_MAXIMIZEBOX | WS_MINIMIZEBOX);
		void setTimer(UINT elapse, UINT id);
		void killTimer(UINT id);
		void setTitle(const std::wstring& title);
		void setSize(float w, float h);
		void setPosition(int x, int y);
		void setCenter();
		void layout();
	public:
		int x{ 0 }, y{ 0 };      // 屏幕坐标：物理像素
		float w{ 0 }, h{ 0 };    // 客户区大小：物理像素
		float dpi{ 1.0 };
		HWND hwnd{ nullptr };
		std::wstring title;
		bool isMouseIn{ false }, wasMaximized{ false };
		std::unique_ptr<Node> body;
		winrt::Windows::UI::Composition::Compositor compositor{ nullptr };
	protected:
		virtual void onCreated() {};
		virtual LRESULT onHitTest(const POINT& pos) { return HTCLIENT; };
		virtual void onMinMaxInfo(MINMAXINFO* mmi) {};
	private:
		std::wstring& getWinClsName(HINSTANCE hIns, const int& iconId);
		static LRESULT CALLBACK winProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
		BOOL setCursor();
		void mouseMove(POINT pos);
		void mouseLeave();
		void mouseDown(POINT pos, bool isRight);
		void mouseUp(POINT pos, bool isRight);
		void mouseWheel(WPARAM wParam, LPARAM lParam);
		void keyDown(UINT key);
		void timer(UINT id);
		void dpiChange(WPARAM wParam, LPARAM lParam);
		void sizeChange(WPARAM wParam, LPARAM lParam);
		void posChange(POINT pos);
	private:	
		winrt::Windows::UI::Composition::Desktop::DesktopWindowTarget winTarget{ nullptr };
	};
}