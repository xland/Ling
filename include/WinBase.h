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
		void show();
		void hide();
		void close();
		void minimize();
		void maximize();
		void restore();
		void refresh();
		void createNativeWindow(int iconId = 0,DWORD exStyle = NULL, DWORD style = WS_POPUP | WS_MAXIMIZEBOX | WS_MINIMIZEBOX);
		void setTimer(UINT elapse, UINT id);
		void killTimer(UINT id);
		void setTitle(const std::wstring& title);
		void setSize(float w, float h);
		void setPosition(int x, int y);
		void setCenter();
		void layout();
		/// <summary>
		/// COMDLG_FILTERSPEC types[] = {
		/// 	{ L"png", L"*.png" },
		/// 	{ L"jpeg", L"*.jpg;*.jpeg" },
		/// 	{ L"all files", L"*.*" }
		/// };
		/// auto path = loader.getFilePath(types);
		/// </summary>
		/// <param name="filter"></param>
		/// <returns></returns>
		std::wstring openFileDialog(std::span<const COMDLG_FILTERSPEC> filter);
	public:
		int x{ 0 }, y{ 0 };
		float w{ 0 }, h{ 0 };
		float dpi{ 1.0 };
		HWND hwnd{ nullptr };
		std::wstring title;
		bool isMouseIn{ false }, isMaximized{ false };
		winrt::Windows::UI::Composition::Compositor compositor{ nullptr };

		// —— 事件字段：外部通过 .add(lambda) 订阅，.remove(token) 退订 ——
		// 声明必须在 body 之前：C++ 按声明的反序析构。子节点 (Button/ScrollerBox) 在
		// 自己的 ~ 里会调用 win->onXxx.remove(token)，如果事件字段先于 body 析构就 UB。
		winrt::event<winrt::delegate<POINT>>        onMouseMove;
		winrt::event<winrt::delegate<POINT, bool>>  onMouseDown;   // (pos, isRight)
		winrt::event<winrt::delegate<POINT, bool>>  onMouseUp;
		winrt::event<winrt::delegate<POINT, float>> onMouseWheel;  // (pos, deltaLogical)
		winrt::event<winrt::delegate<HCURSOR*>>     onCursor;      // handler 写 *cursor
		winrt::event<winrt::delegate<POINT>>        onPosChanged;
		winrt::event<winrt::delegate<UINT>>         onKeyDown;
		winrt::event<winrt::delegate<UINT>>         onTimer;
		winrt::event<winrt::delegate<>>             onSizeChanged;
		winrt::event<winrt::delegate<>>             onDpiChanged;
		winrt::event<winrt::delegate<>>             onDestroy;
		winrt::event<winrt::delegate<>>             onMaximize;
		winrt::event<winrt::delegate<>>             onMinimize;
		winrt::event<winrt::delegate<>>             onRestore;

		std::unique_ptr<Node> body;   // 必须在事件字段之后声明，先于事件字段析构
	protected:
		virtual void onCreated() {};
		virtual LRESULT onHitTest(const POINT pos) { return HTCLIENT; };
		LRESULT borderHitTest(const POINT pt);
		virtual void onMinMaxInfo(MINMAXINFO* mmi);
	private:
		std::wstring& getWinClsName(HINSTANCE hIns, const int& iconId);
		static LRESULT CALLBACK winProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
		BOOL setCursor();
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
