#include "pch.h"
#include "../include/Ling.h"
#include "../include/App.h"

namespace Ling{
	bool init() {
		HRESULT hr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);
		if (FAILED(hr)) return false;
		App::init();
		return true;
	}
	void dispose() {
		App::dispose(); //顺序要紧：先放掉持有 WinRT/COM 对象的单例，再拆套间
		CoUninitialize();
	}
}
