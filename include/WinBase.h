#pragma once
#include <unordered_map>
#include <span>
#include <Windows.h>
#include <shlobj.h>
#include <shlwapi.h>
#include <winrt/base.h>
#include <winrt/Windows.UI.Composition.h>
#include <winrt/Windows.UI.Composition.Desktop.h>
#include <yoga/Yoga.h>

namespace Ling {
	class Node;
	class ScrollerBox;
	class WinBase
	{
		friend class Node;
		friend class ScrollerBox;
	public:
		WinBase();
		~WinBase();
		void enableShadow();
		void enableBorderResize();
		void disableWinAnimation();
		void disableBorderRadius();
		void show();
		void hide();
		void close();
		void minimize();
		void maximize();
		void restore();
		void refresh();
		void createNativeWindow(DWORD exStyle = NULL, DWORD style = WS_POPUP | WS_MAXIMIZEBOX | WS_MINIMIZEBOX);
		void setTimer(UINT elapse, UINT id);
		void killTimer(UINT id);
		void setTitle(const std::wstring& title);
		void setSize(float w, float h);
		void setPosition(int x, int y);
		void setCenter();
		virtual void layout();
		void setMinSize(float w, float h);
		std::wstring openFileDialog(std::span<const COMDLG_FILTERSPEC> filter);
	public:
		int x{ 0 }, y{ 0 };
		float w{ 0 }, h{ 0 }, minW{ 800 }, minH{600};
		float dpi{ 1.0 };
		HWND hwnd{ nullptr };
		std::wstring title;
		bool isMouseIn{ false }, isMaximized{ false };
		winrt::Windows::UI::Composition::Compositor compositor{ nullptr };
		 
		winrt::event<winrt::delegate<POINT>> onMouseMove;
		winrt::event<winrt::delegate<POINT, bool>> onMouseDown; 
		winrt::event<winrt::delegate<POINT, bool>> onMouseUp;
		winrt::event<winrt::delegate<POINT, float>> onMouseWheel; 
		winrt::event<winrt::delegate<bool*>> onCursor; 
		winrt::event<winrt::delegate<UINT>> onKeyDown;
		winrt::event<winrt::delegate<UINT>> onKeyUp;
		// WM_CHAR：参数是字符码（已经过键盘布局与 IME 转换），文本输入取字符要用它而不是 onKeyDown。
		// 码点 > 0xFFFF 时需要使用者自己拼代理对。
		winrt::event<winrt::delegate<UINT>> onChar;
		// WM_IME_STARTCOMPOSITION：输入法开始组字。订阅者应在此把候选框摆到光标附近。
		winrt::event<winrt::delegate<>> onIME;
		// WM_SETFOCUS / WM_KILLFOCUS：窗口级焦点。控件自身的焦点态由控件维护，
		// 窗口失焦时通常要让控件一并失焦（否则光标会在没焦点的窗口里继续闪）。
		winrt::event<winrt::delegate<>> onFocus;
		winrt::event<winrt::delegate<>> onBlur;
		winrt::event<winrt::delegate<UINT>> onTimer;
		winrt::event<winrt::delegate<>> onSizeChanged;
		winrt::event<winrt::delegate<>> onDpiChanged;
		winrt::event<winrt::delegate<>> onDestroy;
		winrt::event<winrt::delegate<>> onMaximize;
		winrt::event<winrt::delegate<>> onMinimize;
		winrt::event<winrt::delegate<>> onRestore;
		winrt::event<winrt::delegate<>> onMoved;

		std::unique_ptr<Node> body;   // 必须在事件字段之后声明，先于事件字段析构
	protected:
		virtual void onCreated() {};
		virtual LRESULT onHitTest(const POINT pos) { return HTCLIENT; };
		LRESULT borderHitTest(const POINT pt);
		virtual void onMinMaxInfo(MINMAXINFO* mmi);
		virtual BOOL setCursor();
	private:
		std::wstring& getWinClsName(HINSTANCE hIns);
		static LRESULT CALLBACK winProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
		void mouseMove(POINT pos);
		void mouseLeave();
		void mouseWheel(WPARAM wParam, LPARAM lParam);
		void dpiChange(WPARAM wParam, LPARAM lParam);
		void sizeChange(WPARAM wParam, LPARAM lParam);
		void posChange(POINT pos);
		int paint();
	private:
		winrt::Windows::UI::Composition::Desktop::DesktopWindowTarget winTarget{ nullptr };
		bool isDirty{ false };
	};
}
