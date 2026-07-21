#include "pch.h"
#include "../include/WinBase.h"

namespace Ling {
	WinBase::WinBase() :Event(), compositor{ Composition::Compositor() }
	{
		dpi = static_cast<float>(GetDpiForSystem()) / 96.f;
	}

	WinBase::~WinBase()
	{
	}

	void WinBase::show()
	{
		if (!hwnd) return;
		ShowWindow(hwnd, SW_SHOW);
	}

	void WinBase::hide()
	{
		if (!hwnd) return;
		ShowWindow(hwnd, SW_HIDE);
	}

	void WinBase::close()
	{
		if (!hwnd) return;
		SetWindowLongPtr(hwnd, GWLP_USERDATA, NULL);
		DestroyWindow(hwnd);
		//emit(EventType::Destroy,nullptr);
	}

	void WinBase::minimize()
	{
		if (!hwnd) return;
		ShowWindow(hwnd, SW_MINIMIZE);
	}

	void WinBase::enableShadow()
	{
		if (!hwnd) return;
		MARGINS margins = { 1,1,1,1 };
		DwmExtendFrameIntoClientArea(hwnd, &margins);
		int value = 2;
		DwmSetWindowAttribute(hwnd, DWMWA_NCRENDERING_POLICY, &value, sizeof(value));
		DwmSetWindowAttribute(hwnd, DWMWA_ALLOW_NCPAINT, &value, sizeof(value));
	}

	void WinBase::setTimer(const UINT& elapse, const UINT& id)
	{
		if (!hwnd) return;
		SetTimer(hwnd, WM_APP + id, elapse, nullptr);
	}

	void WinBase::killTimer(const UINT& id)
	{
		if (!hwnd) return;
		KillTimer(hwnd, WM_APP + id);
	}

	void WinBase::setTitle(const std::wstring& title)
	{
		this->title = title;
		if (hwnd) {
			SetWindowText(hwnd, title.data());
		}
	}

	std::wstring WinBase::getTitle()
	{
		return title;
	}

	std::tuple<int, int> WinBase::getPosition()
	{
		return std::make_tuple(x, y);
	}

	std::tuple<float, float> WinBase::getSize()
	{
		return std::make_tuple(w, h);
	}

	void WinBase::setSize(const float& w, const float& h)
	{
		this->w = w * dpi;
		this->h = h * dpi;
		if (hwnd) {
			SetWindowPos(hwnd, nullptr, 0, 0, int(this->w), int(this->h), SWP_NOMOVE | SWP_NOZORDER | SWP_NOREDRAW);
		}
	}

	void WinBase::setPosition(const int& x, const int& y)
	{
		this->x = x;
		this->y = y;
		if (hwnd) {
			SetWindowPos(hwnd, nullptr, x, y, 0, 0, SWP_NOSIZE | SWP_NOZORDER | SWP_NOREDRAW);
		}
	}

	HWND WinBase::getHWND()
	{
		return hwnd;
	}

	float WinBase::getDPI()
	{
		return dpi;
	}

	void WinBase::setCenter()
	{
		RECT workArea;
		SystemParametersInfo(SPI_GETWORKAREA, 0, &workArea, 0);
		int workWidth = workArea.right - workArea.left;
		int workHeight = workArea.bottom - workArea.top;
		// 工作区左上角偏移（处理多显示器时主屏可能不在 (0,0) 的情况）
		int workLeft = workArea.left;
		int workTop = workArea.top;
		// w/h 现在是物理像素，直接与屏幕坐标做算术
		auto x = workLeft + static_cast<int>((workWidth - w) / 2);
		auto y = workTop + static_cast<int>((workHeight - h) / 2);
		setPosition(x, y);
	}

	void WinBase::createNativeWindow(const DWORD& exStyle, const DWORD& style)
	{
		hwnd = CreateWindowEx(WS_EX_NOREDIRECTIONBITMAP | exStyle, getWinClsName().data(), title.data(), style,
			x, y, static_cast<int>(w), static_cast<int>(h), NULL, NULL, GetModuleHandle(nullptr), NULL); //WS_POPUP
		SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(this));
		auto interop = compositor.as<ABI::Windows::UI::Composition::Desktop::ICompositorDesktopInterop>();
		auto r = reinterpret_cast<ABI::Windows::UI::Composition::Desktop::IDesktopWindowTarget**>(winrt::put_abi(winTarget));
		interop->CreateDesktopWindowTarget(hwnd, false, r);

		visual = compositor.CreateSpriteVisual();
		winTarget.Root(visual);
		visual.Offset({ 0.f,0.f,0.f });
		visual.RelativeSizeAdjustment({ 1.f,1.f });
		onCreated();
	}

	std::wstring& WinBase::getWinClsName()
	{
		static std::wstring clsName = [] {
			WNDCLASSEXW wcex;
			wcex.cbSize = sizeof(WNDCLASSEX);
			wcex.style = CS_HREDRAW | CS_VREDRAW;
			wcex.lpfnWndProc = &WinBase::winProc;
			wcex.hInstance = GetModuleHandle(nullptr);
			wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
			wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
			wcex.lpszMenuName = nullptr;
			wcex.lpszClassName = L"ImageReader";
			wcex.hIcon = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(100));  // 任务栏大图标
			wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(100));  // 标题栏小图标
			auto r = RegisterClassEx(&wcex);
			return wcex.lpszClassName;
			}();
		return clsName;
	}

	LRESULT WinBase::winProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
	{
		auto self = reinterpret_cast<WinBase*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
		if (!self) {
			return DefWindowProc(hwnd, msg, wParam, lParam);
		}
		else if (msg == WM_ERASEBKGND) {
			return 1;
		}
		else if (msg == WM_NCHITTEST) {
			return self->onHitTest((float)((GET_X_LPARAM(lParam) - self->x)), (float)(GET_Y_LPARAM(lParam) - self->y));
		}
		else if (msg == WM_SETCURSOR) {
			if (LOWORD(lParam) == HTCLIENT) return self->setCursor();
		}
		else if (msg == WM_RBUTTONDOWN) {
			self->mouseDown((float)(GET_X_LPARAM(lParam)), (float)(GET_Y_LPARAM(lParam)), true);
		}
		else if (msg == WM_RBUTTONUP) {
			self->mouseUp((float)(GET_X_LPARAM(lParam)), (float)(GET_Y_LPARAM(lParam)), true);
		}
		else if (msg == WM_LBUTTONDOWN) {
			self->mouseDown((float)(GET_X_LPARAM(lParam)), (float)(GET_Y_LPARAM(lParam)), false);
		}
		else if (msg == WM_LBUTTONUP) {
			self->mouseUp((float)(GET_X_LPARAM(lParam)), (float)(GET_Y_LPARAM(lParam)), false);
		}
		else if (msg == WM_MOUSEMOVE) {
			self->mouseMove((float)(GET_X_LPARAM(lParam)), (float)(GET_Y_LPARAM(lParam)));
		}
		else if (msg == WM_MOUSELEAVE) {
			self->mouseMove(FLT_MAX, FLT_MAX);
		}
		else if (msg == WM_MOUSEWHEEL) {
			self->mouseWheel(wParam, lParam);
		}
		else if (msg == WM_KEYDOWN) {
			self->keyDown(wParam);
		}
		else if (msg == WM_TIMER) {
			self->timer(wParam - WM_APP);
		}
		else if (msg == WM_DPICHANGED) {
			self->dpiChange(wParam, lParam);
		}
		else if (msg == WM_SIZE) {
			self->sizeChange(wParam, lParam);
			return 0;
		}
		else if (msg == WM_MOVE) {
			self->posChange(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
		}
		else if (msg == WM_GETMINMAXINFO) {
			self->onMinMaxInfo((PMINMAXINFO)lParam);
		}
		return DefWindowProc(hwnd, msg, wParam, lParam);
	}

	BOOL WinBase::setCursor()
	{
		HCURSOR cursor{ nullptr };
		emit(EventType::Cursor, &cursor);
		if (!cursor) {
			SetCursor(LoadCursor(nullptr, IDC_ARROW));
		}
		return TRUE;
	}

	void WinBase::mouseDown(const float& x, const float& y, bool isRight)
	{
		auto arg = std::make_tuple(x, y, isRight);
		emit(EventType::MouseDown, &arg);
	}

	void WinBase::mouseUp(const float& x, const float& y, bool isRight)
	{
		auto arg = std::make_tuple(x, y, isRight);
		emit(EventType::MouseUp, &arg);
	}

	void WinBase::mouseMove(const float& x, const float& y)
	{
		auto arg = std::make_tuple(x, y);
		emit(EventType::MouseMove, &arg);
	}

	void WinBase::mouseWheel(WPARAM wParam, LPARAM lParam)
	{
		POINT pt{ GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) };
		ScreenToClient(hwnd, &pt);
		auto arg = std::make_tuple((float)pt.x,(float)pt.y, (short)HIWORD(wParam));
		emit(EventType::MouseWheel, &arg);
	}

	void WinBase::keyDown(const UINT& key)
	{
		UINT keyVal = key;
		emit(EventType::KeyDown, &keyVal);
	}

	void WinBase::timer(const UINT& id) 
	{
		UINT idVal = id;
		emit(EventType::Timer, &idVal);
	}

	void WinBase::dpiChange(WPARAM wParam, LPARAM lParam)
	{
		const UINT newDPI = HIWORD(wParam);
		dpi = newDPI / 96.f;
		RECT* prcNewWindow = reinterpret_cast<RECT*>(lParam);
		auto w{ prcNewWindow->right - prcNewWindow->left };
		auto h{ prcNewWindow->bottom - prcNewWindow->top };
		SetWindowPos(hwnd, nullptr, prcNewWindow->left, prcNewWindow->top, w, h, SWP_NOSIZE| SWP_NOREPOSITION | SWP_NOZORDER | SWP_NOACTIVATE);
		posChange(prcNewWindow->left, prcNewWindow->top);
		sizeChange(w, h);
		emit(EventType::DpiChanged, nullptr);
	}

	void WinBase::sizeChange(WPARAM wParam, LPARAM lParam)
	{
		if (wParam == SIZE_MINIMIZED) {
			mouseMove(FLT_MAX, FLT_MAX);
			emit(EventType::Minimize, nullptr);
			return;
		}
		if (wParam == SIZE_MAXIMIZED) {
			wasMaximized = true;
			emit(EventType::Maximize, nullptr);
		}
		else if (wParam == SIZE_RESTORED) {
			if (wasMaximized) {
				wasMaximized = false;
				emit(EventType::Restore, nullptr);
			}
		}
		w = static_cast<float>(GET_X_LPARAM(lParam));
		h = static_cast<float>(GET_Y_LPARAM(lParam));
		if (w <= 0 || h <= 0) return;
		emit(EventType::SizeChanged, nullptr);
	}

	void WinBase::posChange(const int& x, const int& y)
	{
		this->x = x;
		this->y = y;
		auto arg = std::make_tuple(x, y);
		emit(EventType::PosChanged, nullptr);
	}
}