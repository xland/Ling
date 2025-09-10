#pragma once
#include <Windows.h>
#include "WindowBase.h"
namespace tvg {
	class SwCanvas;
}
namespace Ling {
	class WindowLayered :public WindowBase
	{
		public:
			void createNativeWindow() override;
			void repaint();
			void show() override;
		protected:
			LRESULT CALLBACK customMsgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) override;
		private:
			LRESULT hitTest(int x, int y);
			void scalePix();
			tvg::SwCanvas* scaledCanvas;
			std::vector<uint32_t> scaledBuffer;
	};
}

