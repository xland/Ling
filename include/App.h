#pragma once
#include <dispatcherqueue.h>
#include <winrt/Windows.System.h>
#include "Ling.h"

namespace Ling {
	class App
	{
		public:
			~App();
			static App* get();
		private:
			static void init();
			App();
			friend void Ling::init();
		private:
			static void initDispatcherQueueCtrl();
			winrt::Windows::System::DispatcherQueue dq;
	};
}