#include "pch.h"
#include "../include/Ling.h"
#include "../include/App.h"
#include "../include/D2D.h"

namespace Ling{
	bool init() {
		HRESULT hr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);
		if (FAILED(hr)) return false;
		App::init();
		return true;
	}
	void dispose() {
		D2D::dispose(); //正常情况下最后一个窗口关掉时就没了，这里兜住"还开着窗口就退出"那条路
		App::dispose(); //顺序要紧：先放掉持有 WinRT/COM 对象的单例，再拆套间
		CoUninitialize();
	}
}
