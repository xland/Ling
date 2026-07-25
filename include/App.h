#pragma once
#include <dispatcherqueue.h>
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
			void refuseSecondInstance(const std::wstring& checkId);
		private:
			static void init();
			App();
			friend bool Ling::init();
		private:
			void initMsgWin(const std::wstring& wndName);
			static LRESULT CALLBACK winProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
			static void initDispatcherQueueCtrl();
			winrt::Windows::System::DispatcherQueue dq;
			HWND msgHwnd;
			
	};
}