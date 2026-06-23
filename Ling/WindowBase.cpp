import Ling;
#include "WindowBase.h"

WindowBase::WindowBase():compositor{ winrt::Windows::UI::Composition::Compositor() }
{

}

WindowBase::~WindowBase()
{}

void WindowBase::show()
{
    ShowWindow(hwnd, SW_SHOW);
    //SetForegroundWindow(hwnd);
    //SetFocus(hwnd);
    //UpdateWindow(hwnd);
}
void WindowBase::hide()
{
	ShowWindow(hwnd, SW_HIDE);
}
void WindowBase::refresh()
{
    InvalidateRect(hwnd, nullptr, false);
}
void WindowBase::close()
{
    SetWindowLongPtr(hwnd, GWLP_USERDATA, NULL);
    DestroyWindow(hwnd);
    onDestroy();
}
void WindowBase::move(const int& x, const int& y)
{
	this->x = x;
	this->y = y;
	SetWindowPos(hwnd, nullptr, x, y, 0, 0, SWP_NOSIZE | SWP_NOZORDER| SWP_NOREDRAW);
}

void WindowBase::resize(const int& w, const int& h)
{
	this->w = w;
	this->h = h;
	SetWindowPos(hwnd, nullptr, 0, 0, w, h, SWP_NOMOVE | SWP_NOZORDER| SWP_NOREDRAW);
}

void WindowBase::enableShadow()
{
    MARGINS margins = { 1,1,1,1 };
    DwmExtendFrameIntoClientArea(hwnd, &margins);
    int value = 2;
    DwmSetWindowAttribute(hwnd, DWMWA_NCRENDERING_POLICY, &value, sizeof(value));
    DwmSetWindowAttribute(hwnd, DWMWA_ALLOW_NCPAINT, &value, sizeof(value));
    DWM_WINDOW_CORNER_PREFERENCE preference = DWMWCP_DONOTROUND;
    DwmSetWindowAttribute(hwnd, DWMWA_WINDOW_CORNER_PREFERENCE, &preference, sizeof(preference));
}
void WindowBase::setTimer(const UINT& elapse, const UINT& id)
{
    SetTimer(hwnd, WM_APP + id, elapse, nullptr);
}

void WindowBase::killTimer(const UINT& id)
{
    KillTimer(hwnd, WM_APP + id);
}

void WindowBase::setCursor(LPCWSTR cursorName)
{
    SetCursor(LoadCursor(NULL, cursorName));
}


void WindowBase::createWindow(const DWORD& exStyle, const DWORD& style)
{
    hwnd = CreateWindowEx(WS_EX_NOREDIRECTIONBITMAP| exStyle, getWinClsName().c_str(), NULL, style, x, y, w, h, NULL, NULL, GetModuleHandle(nullptr), NULL); //WS_POPUP
    SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(this));
	dpi = GetDpiForWindow(hwnd) / 96.0f;
    auto interop = compositor.as<ABI::Windows::UI::Composition::Desktop::ICompositorDesktopInterop>();
    interop->CreateDesktopWindowTarget(
        hwnd, false,
        reinterpret_cast<ABI::Windows::UI::Composition::Desktop::IDesktopWindowTarget**>(winrt::put_abi(winTarget))
    );
    rootVisual = compositor.CreateContainerVisual();
    rootVisual.RelativeSizeAdjustment({ 1.0f, 1.0f });
    rootVisual.Offset({ 0, 0, 0 });    
    winTarget.Root(rootVisual);



    element = compositor.CreateSpriteVisual();
    element.RelativeSizeAdjustment({ 1.0f, 1.0f });
    //element.Size({ (float)50, (float)50 });
    //element.Offset({ (float)w - (float)100, (float)y - (float)100, (float)0 });

    winrt::Windows::UI::Composition::CompositionColorBrush brush = compositor.CreateColorBrush(winrt::Windows::UI::Colors::Red());
    element.Brush(brush);

    // 将 sprite 添加到 root 的子节点中
    rootVisual.Children().InsertAtTop(element);

    onCreated();
}

LRESULT WindowBase::onHitTest(WPARAM wParam, LPARAM lParam)
{
    return DefWindowProc(hwnd, WM_NCHITTEST, wParam, lParam);
}

BOOL WindowBase::onCursor()
{
    SetCursor(LoadCursor(NULL, IDC_CROSS));
    return TRUE;
}
std::wstring& WindowBase::getWinClsName()
{
    static std::wstring clsName = [] {
        WNDCLASSEXW wcex;
        wcex.cbSize = sizeof(WNDCLASSEX);
        wcex.style = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
        wcex.lpfnWndProc = &WindowBase::winProc;
        wcex.cbClsExtra = 0;
        wcex.cbWndExtra = 0;
        wcex.hInstance = GetModuleHandle(nullptr);
        wcex.hIcon = LoadIcon(wcex.hInstance, (LPCTSTR)IDI_WINLOGO);
        wcex.hCursor = LoadCursor(nullptr, IDC_CROSS);
        wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
        wcex.lpszMenuName = nullptr;
        wcex.lpszClassName = L"HorseJs";
        wcex.hIconSm = LoadIcon(wcex.hInstance, (LPCTSTR)IDI_WINLOGO);
        auto r = RegisterClassEx(&wcex);
        if (r == 0) {
            log(L"err:: reg window class error");
        }
        return wcex.lpszClassName;
        }();
    return clsName;
}

LRESULT WindowBase::winProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    auto self = reinterpret_cast<WindowBase*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
    if (!self) {
        return DefWindowProc(hwnd, msg, wParam, lParam);
    }
    else if (msg == WM_MOUSEACTIVATE) {
        return MA_NOACTIVATE;
    }
    else if (msg == WM_NCHITTEST)
    {
        return self->onHitTest(wParam, lParam);
    }
    else if (msg == WM_ERASEBKGND)
    {
        return 1;
    }
    else if (msg == WM_SHOWWINDOW) {
        if (wParam) {
            self->onShown();
        }
        else {
            self->onHidden();
        }
    }
    else if (msg == WM_SETCURSOR) {
        if (LOWORD(lParam) == HTCLIENT) return self->onCursor();
    }
    else if (msg == WM_RBUTTONDOWN)
    {
        self->onMouseDown(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam), true);
    }
    else if (msg == WM_RBUTTONDBLCLK)
    {
        self->onMouseDoubleClick(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam), true);
        return 0;
    }
    else if (msg == WM_LBUTTONDBLCLK)
    {
        self->onMouseDoubleClick(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam), false);
        return 0;
    }
    else if (msg == WM_LBUTTONDOWN) {
        SetCapture(hwnd);
        self->isMouseDown = true;
        self->onMouseDown(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam), false);
    }
    else if (msg == WM_LBUTTONUP) {
        ReleaseCapture();
        self->isMouseDown = false;
        self->onMouseUp(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
    }
    else if (msg == WM_MOUSEMOVE) {
        if (self->isMouseDown) {
            self->onMouseDrag(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam), wParam);
        }
        else {
            self->mouseMove(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
        }
    }
    else if (msg == WM_MOUSELEAVE) {
        self->mouseLeave();
        return 0;
    }
    else if (msg == WM_MOUSEWHEEL) {
        POINT pt{ (short)GET_X_LPARAM(lParam), (short)GET_Y_LPARAM(lParam) };
        ScreenToClient(hwnd, &pt);
        self->onMouseWheel(pt.x, pt.y, (short)HIWORD(wParam));
        return 0;
    }
    else if (msg == WM_KEYDOWN) {
        self->onKeyDown(wParam);
    }
    else if (msg == WM_KEYUP) {
        self->onKeyUp();
    }
    else if (msg == WM_CHAR) {
        self->onChar(wParam);
    }
    else if (msg == WM_IME_STARTCOMPOSITION) {
        self->onIME();
    }
    else if (msg == WM_TIMER) {
        self->onTimer(wParam-WM_APP);
    }
    else if (msg == WM_KILLFOCUS) {
        self->onBlur();
    }
    else if (msg == WM_DPICHANGED) {
        self->onDpiChanged();
        return 0;
    }
    else if (msg == WM_SIZE) {
        self->w = LOWORD(lParam);
        self->h = HIWORD(lParam);
        self->element.Offset({ (float)(self->w - 100), (float)(self->h - 100), 0 });
        return 0;
    }
    return DefWindowProc(hwnd, msg, wParam, lParam);
}

void WindowBase::mouseMove(const int& x, const int& y)
{
    if (!isMouseIn)
    {
        isMouseIn = true;
        TRACKMOUSEEVENT tme{ sizeof(TRACKMOUSEEVENT) };
        tme.dwFlags = TME_LEAVE;
        tme.hwndTrack = hwnd;
        TrackMouseEvent(&tme);
    }
	onMouseMove(x, y);
}

void WindowBase::mouseLeave()
{
	isMouseIn = false;
    TRACKMOUSEEVENT tme{ sizeof(TRACKMOUSEEVENT) };
    tme.dwFlags = TME_CANCEL | TME_LEAVE;
    tme.hwndTrack = hwnd;
    TrackMouseEvent(&tme);
	onMouseLeave();
}

