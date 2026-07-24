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
		private:
			static void init();
			App();
			friend bool Ling::init();
		private:
			static void initDispatcherQueueCtrl();
			winrt::Windows::System::DispatcherQueue dq;
	};
}