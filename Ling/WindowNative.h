#pragma once
#include "WindowElementManager.h"

namespace Ling {
	class WindowNative:public WindowElementManager
	{
	public:
		WindowNative();
		~WindowNative();
		void enableShadow();
		void show();
		void hide();
		void refresh();
		void close();
		void move(const int& x, const int& y);
		void resize(const int& w, const int& h);
		void createNativeWindow(const DWORD& exStyle = NULL, const DWORD& style = NULL);
		void setTimer(const UINT& elapse, const UINT& id);
		void killTimer(const UINT& id);
		void setTitle(const std::wstring& title);
		std::wstring getTitle();
		std::tuple<int, int> getPosition();
		std::tuple<float, float> getSize();
		void setSize(const float& w, const float& h);
		void setPosition(const int& xWin, const int& yWin);
		HWND getHandle();
		float getScaleFactor();
		void setPosScreenCenter();
	public:
		int x, y, w, h;
		bool isMouseDown{ false }, isMouseIn{ false };
		float dpi{ 1.0 };
	protected:
		virtual void onCreated() {};
		virtual void onShown() {};
		virtual void onHidden() {};
		virtual LRESULT onHitTest(WPARAM wParam, LPARAM lParam);
		virtual void onMouseDrag(const int& x, const int& y, const UINT_PTR& modifiers) {};
		virtual void onMouseDown(const int& x, const int& y, bool isRight) {};
		virtual void onMouseDoubleClick(const int& x, const int& y, bool isRight) { onMouseDown(x, y, isRight); };
		virtual void onMouseUp(const int& x, const int& y) {};
		virtual void onKeyDown(const UINT& key) {};
		virtual void onKeyUp() {};
		virtual void onChar(const UINT& ch) {};
		virtual void onMouseWheel(const int& x, const int& y, const short& delta) {};
		virtual void onTimer(const UINT& timerId) {};
		virtual void onIme() {};
		virtual void onBlur() {};
		virtual void onDestroy() {};
		virtual void onDpiChanged() {};
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
	};
}


