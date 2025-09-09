#pragma once
#include <Windows.h>
#include "WindowBase.h"

namespace Ling {
	class WindowLayered :public WindowBase
	{
		public:
			void createNativeWindow() override;
			void repaint();
			void show() override;
		protected:
		private:
	};
}

