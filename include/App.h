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
			void refuseSecondInstance();
			void regHotKey(const std::wstring& keyStr, const UINT msgId);
			void initTray(const UINT msgId,const std::wstring& tip);
			void disposeTray();
			winrt::event<winrt::delegate<>> onSecondInstance;
			winrt::event<winrt::delegate<UINT>> onHotKey;
			winrt::event<winrt::delegate<bool,bool,bool>> onTrayMouseEvent;
			std::wstring appID;
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