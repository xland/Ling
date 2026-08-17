#pragma once
#include <dispatcherqueue.h>
#include <shellapi.h>
#include <vector>
#include <winrt/Windows.System.h>
namespace Ling {
	bool init();
	void dispose();
	class WinBase;
	class App
	{
		public:
			~App();
			static App* get();
			void quit(int code = 0);
			void exit(int code = 0);
			/// <summary>
			/// 如果发现系统中已经存在一个应用实例，
			/// 则当前应用马上退出，此方法返回true
			/// 并触发前一个应用实例的onSecondInstance事件
			/// </summary>
			bool refuseSecondInstance();
			/// <summary>
			/// 注册全局热键，返回是否注册成功。键名认不出来、或者这个组合已经被别的程序占了
			/// 都会失败 —— 失败要不要告诉用户由调用方决定
			/// </summary>
			bool regHotKey(const std::wstring& keyStr, const UINT msgId);
			void unRegHotKey(const UINT msgId);
			void initTray(const UINT msgId,const std::wstring& tip);
			void disposeTray();
			void initArgs();
			UINT popupMenu(HMENU menu);
		public:
			winrt::event<winrt::delegate<>> onBeforeQuit;
			winrt::event<winrt::delegate<>> onSecondInstance;
			winrt::event<winrt::delegate<UINT>> onHotKey;
			winrt::event<winrt::delegate<bool,bool>> onTrayMouseEvent;
			std::wstring appID;
			std::unordered_map<std::wstring, std::wstring> args;
			winrt::Windows::System::DispatcherQueue dq;
			// 当前打开的窗口，按创建顺序。WinBase 构造时登记、析构时注销。
			// 最后一个窗口注销时会顺手销毁 D2D 设备 —— 一个窗口都没有时它没有任何用处
			std::vector<WinBase*> windows;
		private:
			static void init();
			// 由 Ling::dispose 调用：托盘图标、DispatcherQueue 都得在 CoUninitialize 之前放掉，
			// 留给进程退出后的静态析构就是往已经拆掉的 COM 套间上打 Release
			static void dispose();
			App();
			friend bool Ling::init();
			friend void Ling::dispose();
		private:
			void initMsgWin();
			static LRESULT CALLBACK winProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
			static void initDispatcherQueueCtrl();
			HWND msgHwnd;
			std::unique_ptr<NOTIFYICONDATA> tray;
			UINT trayMsgId;
	};
}