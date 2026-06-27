#pragma once
#include "Header.h"
#include <yoga/Yoga.h>
namespace Ling {
	class WindowBase;
	class WindowNative
	{
	public:
		WindowNative(WindowBase* base);
		~WindowNative();
		void enableShadow();
		void show();
		void hide();
		void refresh();
		void close();
		void move(const int& x, const int& y);
		void resize(const int& w, const int& h);
		void createNativeWindow(const DWORD& exStyle = NULL, const DWORD& style = NULL);
	public:
		int x, y, w, h;
		bool isMouseDown{ false }, isMouseIn{ false };
		Composition::Compositor compositor;
		float dpi{ 1.0 };
	private:
		std::wstring& getWinClsName();
		static LRESULT CALLBACK winProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
		void mouseMove(const int& x, const int& y);
		void mouseLeave();
		void paint();
		void dpiChange(WPARAM wParam, LPARAM lParam);
		void sizeChange(const int& w, const int& h);
	private:
		HWND hwnd{ nullptr };
		std::wstring title;
		Composition::Desktop::DesktopWindowTarget winTarget{ nullptr };
		WindowBase* base;
	};
}


