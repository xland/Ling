#include "pch.h"
#include "../include/Ling.h"
#include "../include/App.h"

namespace Ling{
	void init() {
		HRESULT hr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);
		if (FAILED(hr)) return;
		App::init();
	}
	void dispose() {
		CoUninitialize();
	}
}
