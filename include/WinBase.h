#pragma once
#include <Windows.h>
#include <winrt/Windows.UI.Composition.h>
#include <winrt/Windows.UI.Composition.Desktop.h>
#include "Event.h"

namespace Ling {
	class WinBase:public Event
	{
	public:
		WinBase();
		~WinBase();
		void enableShadow();
		void show();
		void hide();
		void close();
		void minimize();
		void createNativeWindow(const DWORD& exStyle = NULL, const DWORD& style = NULL);
		void setTimer(const UINT& elapse, const UINT& id);
		void killTimer(const UINT& id);
		void setTitle(const std::wstring& title);
		std::wstring getTitle();
		std::tuple<int, int> getPosition();
		std::tuple<float, float> getSize();
		void setSize(const float& w, const float& h);
		void setPosition(const int& x, const int& y);
		HWND getHWND();
		float getDPI();
		void setCenter();
	protected:
		virtual void onCreated() {};
		virtual LRESULT onHitTest(const float& x, const float& y) { return HTCLIENT; };
		virtual void onMinMaxInfo(MINMAXINFO* mmi) {};
	private:
		std::wstring& getWinClsName();
		static LRESULT CALLBACK winProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
		BOOL setCursor();
		void mouseMove(const float& x, const float& y);
		void mouseDown(const float& x, const float& y, bool isRight);
		void mouseUp(const float& x, const float& y, bool isRight);
		void mouseWheel(WPARAM wParam, LPARAM lParam);
		void keyDown(const UINT& key);
		void timer(const UINT& id);
		void dpiChange(WPARAM wParam, LPARAM lParam);
		void sizeChange(WPARAM wParam, LPARAM lParam);
		void posChange(const int& x, const int& y);
	private:	
		int x{ 0 }, y{ 0 };      // 屏幕坐标：物理像素
		float w{ 0 }, h{ 0 };    // 客户区大小：物理像素
		float dpi{ 1.0 };
		HWND hwnd{ nullptr };
		std::wstring title;
		winrt::Windows::UI::Composition::Desktop::DesktopWindowTarget winTarget{ nullptr };
		winrt::Windows::UI::Composition::Compositor compositor{nullptr};
		Composition::SpriteVisual visual{ nullptr };
		bool isMouseIn{ false }, wasMaximized{ false };
	};
}