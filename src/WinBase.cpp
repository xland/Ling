#include "pch.h"
#include "../include/WinBase.h"
#include "../include/Node.h"
#include "../include/NodeScroller.h"

namespace Ling {
	WinBase::WinBase() :EventBase(), compositor{ Composition::Compositor() }
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
		emit(Event::Destroy, nullptr);
	}

	void WinBase::minimize()
	{
		if (!hwnd) return;
		ShowWindow(hwnd, SW_MINIMIZE);
	}

	void WinBase::maximize()
	{
		if (!hwnd) return;
		ShowWindow(hwnd, SW_MAXIMIZE);
	}

	void WinBase::restore()
	{
		if (!hwnd) return;
		ShowWindow(hwnd, SW_RESTORE);
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

	void WinBase::setTimer(UINT elapse, UINT id)
	{
		if (!hwnd) return;
		SetTimer(hwnd, WM_APP + id, elapse, nullptr);
	}

	void WinBase::killTimer(UINT id)
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

	void WinBase::setSize(float w, float h)
	{
		this->w = w * dpi;
		this->h = h * dpi;
		if (hwnd) {
			SetWindowPos(hwnd, nullptr, 0, 0, int(this->w), int(this->h), SWP_NOMOVE | SWP_NOZORDER | SWP_NOREDRAW);
		}
	}

	void WinBase::setPosition(int x, int y)
	{
		this->x = x;
		this->y = y;
		if (hwnd) {
			SetWindowPos(hwnd, nullptr, x, y, 0, 0, SWP_NOSIZE | SWP_NOZORDER | SWP_NOREDRAW);
		}
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

	void WinBase::layout()
	{
		body->_beforeLayout();
		// body 是根节点，尺寸完全由窗口物理尺寸决定 —— 直接设给 yoga，不走 Node::setSize （那会把 w/h 当成逻辑值再乘一遍 dpi，导致翻倍）
		YGNodeStyleSetWidth(body->node, w);
		YGNodeStyleSetHeight(body->node, h);
		YGNodeCalculateLayout(body->node, w, h, YGDirectionLTR);
		body->layout();
	}

	void WinBase::createNativeWindow(int iconId, DWORD exStyle, DWORD style)
	{
		auto hIns = GetModuleHandle(nullptr);
		auto cls = getWinClsName(hIns,iconId);
		hwnd = CreateWindowEx(WS_EX_NOREDIRECTIONBITMAP | exStyle, cls.data(), title.data(), style, x, y, (float)w, (float)h, NULL, NULL, hIns, NULL); //WS_POPUP
		SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(this));
		auto interop = compositor.as<ABI::Windows::UI::Composition::Desktop::ICompositorDesktopInterop>();
		auto r = reinterpret_cast<ABI::Windows::UI::Composition::Desktop::IDesktopWindowTarget**>(winrt::put_abi(winTarget));
		interop->CreateDesktopWindowTarget(hwnd, false, r);
		body = std::unique_ptr<Node>(new Node(this));
		winTarget.Root(body->visual);
		onCreated();
		layout();
	}

	std::wstring& WinBase::getWinClsName(HINSTANCE hIns, const int& iconId)
	{
		static std::wstring clsName = [&hIns,&iconId] {
			WNDCLASSEXW wcex{};
			wcex.cbSize = sizeof(WNDCLASSEX);
			wcex.style = CS_HREDRAW | CS_VREDRAW;
			wcex.lpfnWndProc = &WinBase::winProc;
			wcex.hInstance = hIns;
			wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
			wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
			wcex.lpszMenuName = nullptr;
			wcex.lpszClassName = L"Ling";
			if (iconId > 0) {
				wcex.hIcon   = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(iconId));
				wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(iconId));
			}
			else {
				wcex.hIcon   = LoadIcon(nullptr, IDI_APPLICATION);
				wcex.hIconSm = LoadIcon(nullptr, IDI_APPLICATION);
			}
			ATOM atom = RegisterClassEx(&wcex);
			if (atom == 0) {
				assert("RegisterClassEx failed");
			}
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
			return self->onHitTest({ GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) });
		}
		else if (msg == WM_SETCURSOR) {
			if (LOWORD(lParam) == HTCLIENT) return self->setCursor();
		}
		else if (msg == WM_RBUTTONDOWN) {
			self->mouseDown({ GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) }, true);
		}
		else if (msg == WM_RBUTTONUP) {
			self->mouseUp({ GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) }, true);
		}
		else if (msg == WM_LBUTTONDOWN) {
			self->mouseDown({ GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) }, false);
		}
		else if (msg == WM_LBUTTONUP) {
			self->mouseUp({ GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) }, false);
		}
		else if (msg == WM_MOUSEMOVE) {
			self->mouseMove({ GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) });
		}
		else if (msg == WM_MOUSELEAVE) {
			self->mouseLeave();
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
			self->posChange({ GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) });
		}
		else if (msg == WM_GETMINMAXINFO) {
			self->onMinMaxInfo((PMINMAXINFO)lParam);
		}
		return DefWindowProc(hwnd, msg, wParam, lParam);
	}

	BOOL WinBase::setCursor()
	{
		HCURSOR cursor{ nullptr };
		emit(Event::Cursor, &cursor);
		if (!cursor) {
			SetCursor(LoadCursor(nullptr, IDC_ARROW));
		}
		return TRUE;
	}

	void WinBase::mouseDown(POINT pos, bool isRight)
	{
		auto arg = std::make_tuple(pos, isRight);
		emit(Event::MouseDown, &arg);
	}

	void WinBase::mouseUp(POINT pos, bool isRight)
	{
		auto arg = std::make_tuple(pos, isRight);
		emit(Event::MouseUp, &arg);
	}

	void WinBase::mouseMove(POINT pos)
	{
		if (!isMouseIn) {
			isMouseIn = true;
			TRACKMOUSEEVENT tme{ sizeof(TRACKMOUSEEVENT) };
			tme.dwFlags = TME_LEAVE;
			tme.hwndTrack = hwnd;
			TrackMouseEvent(&tme);
		}
		auto arg = std::make_tuple(pos);
		emit(Event::MouseMove, &arg);
	}

	void WinBase::mouseLeave()
	{
		isMouseIn = false;
		auto arg = std::make_tuple(FLT_MAX, FLT_MAX);
		emit(Event::MouseMove, &arg);
	}

	void WinBase::mouseWheel(WPARAM wParam, LPARAM lParam)
	{
		POINT pt{ GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) };
		ScreenToClient(hwnd, &pt);
		auto delta{ (short)HIWORD(wParam) };
		auto space = (delta / (float)WHEEL_DELTA) * 60.f * dpi;
		auto arg = std::make_tuple(pt, space);
		emit(Event::MouseWheel, &arg);
	}

	void WinBase::keyDown(UINT key)
	{
		UINT keyVal = key;
		emit(Event::KeyDown, &keyVal);
	}

	void WinBase::timer(UINT id) 
	{
		UINT idVal = id;
		emit(Event::Timer, &idVal);
	}

	void WinBase::dpiChange(WPARAM wParam, LPARAM lParam)
	{
		const UINT newDPI = HIWORD(wParam);
		dpi = newDPI / 96.f;
		body->applyDpiChange();
		emit(Event::DpiChanged, nullptr);
		RECT* prcNewWindow = reinterpret_cast<RECT*>(lParam);
		const int newW = prcNewWindow->right - prcNewWindow->left;
		const int newH = prcNewWindow->bottom - prcNewWindow->top;
		SetWindowPos(hwnd, nullptr, prcNewWindow->left, prcNewWindow->top, newW, newH, SWP_NOZORDER | SWP_NOACTIVATE);
	}

	void WinBase::sizeChange(WPARAM wParam, LPARAM lParam)
	{
		if (wParam == SIZE_MINIMIZED) {
			mouseMove(POINT{INT_MAX,INT_MAX});
			emit(Event::Minimize, nullptr);
			return;
		}
		if (wParam == SIZE_MAXIMIZED) {
			isMaximized = true;
			emit(Event::Maximize, nullptr);
		}
		else if (wParam == SIZE_RESTORED) {
			if (isMaximized) {
				isMaximized = false;
				emit(Event::Restore, nullptr);
			}
		}
		w = static_cast<float>(GET_X_LPARAM(lParam));
		h = static_cast<float>(GET_Y_LPARAM(lParam));
		if (w <= 0 || h <= 0) return;
		emit(Event::SizeChanged, nullptr);
		layout();
	}

	void WinBase::posChange(POINT pos)
	{
		this->x = pos.x;
		this->y = pos.y;
		auto arg = std::make_tuple(pos);
		emit(Event::PosChanged, nullptr);
	}
}