#pragma once
#include <Windows.h>
#include "WindowBase.h"

namespace Ling {
	class WindowNormal :public WindowBase
	{
		public:
			void createNativeWindow() override;
		protected:
		private:
	};
}

