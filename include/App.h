#pragma once
#include <dispatcherqueue.h>
#include <shellapi.h>
#include <winrt/Windows.System.h>
namespace Ling {
	bool init();
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
			void regHotKey(const std::wstring& keyStr, const UINT msgId);
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
		private:
			static void init();
			App();
			friend bool Ling::init();
		private:
			void initMsgWin();
			static LRESULT CALLBACK winProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
			static void initDispatcherQueueCtrl();
			winrt::Windows::System::DispatcherQueue dq;
			HWND msgHwnd;
			std::unique_ptr<NOTIFYICONDATA> tray;
			UINT trayMsgId;
	};
}