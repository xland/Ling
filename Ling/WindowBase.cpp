#include <yoga/Yoga.h>

#include "Element.h"
#include "App.h"
#include "WindowBase.h"
#include "private\WindowBaseImpl.h"

WindowBase::WindowBase() :winPosition(0, 0), winSize(980, 680)
{    
}
WindowBase::~WindowBase() {

}
void WindowBase::show() {
    ShowWindow(hwnd, SW_SHOW);     // 或者 SW_SHOWNORMAL / SW_SHOWDEFAULT
    UpdateWindow(hwnd);
    casecadeShown();
}

void WindowBase::resetWindowToScreenCenter()
{
    int screenWidth = GetSystemMetrics(SM_CXSCREEN);
    int screenHeight = GetSystemMetrics(SM_CYSCREEN);
    auto size = getWindowSize();
    int x{ (screenWidth - size.w) / 2 };
    int y{ (screenHeight - size.h) / 2 };
    setWindowPosition(x,y);
    SetWindowPos(hwnd, NULL, x, y, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
}

void WindowBase::setWindowToScreenCenter()
{
    int screenWidth = GetSystemMetrics(SM_CXSCREEN);
    int screenHeight = GetSystemMetrics(SM_CYSCREEN);
    auto size = getWindowSize();
    int x{ (screenWidth - size.w) / 2 };
    int y{ (screenHeight - size.h) / 2 };
    setWindowPosition(x, y);
}

void WindowBase::setTitle(const std::wstring& title)
{
    this->title = title;
}

const std::wstring& WindowBase::getTitle()
{
    return title;
}

void WindowBase::insertChild(const int& index, Element* ele)
{
    ElementBox::insertChild(index, ele);
    casecadeSetWindow(ele);
}

void WindowBase::addChild(Element* ele)
{
    ElementBox::addChild(ele);
    casecadeSetWindow(ele);
}

void WindowBase::casecadeSetWindow(Element* ele)
{
    ele->win = this;
    auto box = dynamic_cast<ElementBox*>(ele);
    if (box) {
        for (auto e: *(box->getChildren()))
        {
            casecadeSetWindow(e);
        }
    }
}

const Position& WindowBase::getWindowPosition()
{
    return winPosition;
}

const Size& WindowBase::getWindowSize()
{
    return winSize;
}

const Size WindowBase::getWindowClientSize()
{
    RECT clientRect;
    GetClientRect(hwnd, &clientRect);
    Size s(clientRect.right - clientRect.left, clientRect.bottom - clientRect.top);
    return s;
}

void WindowBase::resetWindowSize(const int& w, const int& h)
{
    winSize.w = w;
    winSize.h = h;
    SetWindowPos(hwnd, NULL, 0, 0, w, h, SWP_NOMOVE | SWP_NOZORDER);
}

void WindowBase::resetWindowPosition(const int& x, const int& y)
{
    winPosition.x = x;
    winPosition.y = y;
    SetWindowPos(hwnd, NULL, x, y, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
}
void WindowBase::setWindowSize(const int& w, const int& h)
{
    winSize.w = w;
    winSize.h = h;
}

void WindowBase::setWindowPosition(const int& x, const int& y)
{
    winPosition.x = x;
    winPosition.y = y;
}

LRESULT CALLBACK WindowBase::routeWindowMsg(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    auto winObj = reinterpret_cast<WindowBase*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
    if (winObj)
    {
        return winObj->windowMsgProc(hwnd, msg, wParam, lParam);
    }
    else {
        return DefWindowProc(hwnd, msg, wParam, lParam);
    }
}

LRESULT CALLBACK WindowBase::windowMsgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) 
{
    switch (msg)
    {
    case WM_NCDESTROY:
    {
        SetWindowLongPtr(hwnd, GWLP_USERDATA, 0);
        PostQuitMessage(0);
        return 0;
    }
    case WM_DESTROY:
    {
        SetWindowLongPtr(hwnd, GWLP_USERDATA, 0);
        PostQuitMessage(0);
        return 0;
    }
    case WM_MOVE: 
    {
        setWindowPosition(LOWORD(lParam), HIWORD(lParam));        
        return 0;
    }
    case WM_SIZE:
    {
        int w{ LOWORD(lParam) }, h{ HIWORD(lParam) };
        if (w == 0 || h == 0) {
            return 0;
        }
        setWindowSize(w, h);
        winImpl->reset();
        layout();
        return 0;
    }
    case WM_DPICHANGED:
    {
        scaleFactor = LOWORD(wParam) / 96.0f;
        RECT* suggestedRect = reinterpret_cast<RECT*>(lParam);
        auto w{ suggestedRect->right - suggestedRect->left };
        auto h{ suggestedRect->bottom - suggestedRect->top };
        SetWindowPos(hwnd, nullptr, suggestedRect->left, suggestedRect->top, w, h,
            SWP_NOZORDER | SWP_NOACTIVATE);
        //setWindowSize(w, h);
        //winImpl->reset();
        //layout();
        return 0;
    }
    //case WM_SETCURSOR: {
    //    return 1;
    //}
    case WM_ERASEBKGND:
    {
        return 1;
    }
    case WM_TIMER: {
        if (wParam == FlashCaretTimer) {
            auto a = 1;
        }
        return 0;
    }
    case WM_PAINT: {
        winImpl->paintElement(this);
        paintArea();
        return 0;
    }
    case WM_LBUTTONDBLCLK:
    {
        return 0;
    }
    case WM_MOUSEMOVE:
    {
        windowMouseMove(LOWORD(lParam), HIWORD(lParam));
        return 0;
    }
    case WM_LBUTTONDOWN:
    {
        windowMouseDown(LOWORD(lParam), HIWORD(lParam), MouseButton::Left);
        return 0;
    }
    case WM_LBUTTONUP:
    {
        windowMouseUp(LOWORD(lParam), HIWORD(lParam), MouseButton::Left);
        return 0;
    }
    case WM_RBUTTONDOWN:
    {
        windowMouseDown(LOWORD(lParam), HIWORD(lParam), MouseButton::Right);
        return 0;
    }
    case WM_RBUTTONUP:
    {
        windowMouseUp(LOWORD(lParam), HIWORD(lParam), MouseButton::Right);
        return 0;
    }
    case WM_KEYDOWN:
    {
        switch (wParam)
        {
        case VK_DELETE: {
            return 0;
        }
        case VK_LEFT: {
            return 0;
        }
        case VK_RIGHT: {
            return 0;
        }
        case VK_ESCAPE: {
            return 0;
        }
        case VK_SHIFT: {
            return 0;
        }
        case VK_CONTROL: {
            return 0;
        }
        }
        return 0;
    }
    case WM_KEYUP:
    {
        switch (wParam)
        {
        case VK_SHIFT: {
            return 0;
        }
        case VK_CONTROL: {
            return 0;
        }
        }
        return 0;
    }
    case WM_CHAR:
    {
        return 1;
    }
    case WM_IME_STARTCOMPOSITION:
    {
        break;
    }
    case WM_IME_ENDCOMPOSITION:
    {
        break;
    }
    case WM_IME_NOTIFY:
    {
        switch (wParam)
        {
        case IMN_SETOPENSTATUS:
            break;
        default:
            break;
        }
        break;
    }
    }
    return customMsgProc(hwnd, msg, wParam, lParam);
}

void WindowBase::windowMouseMove(const int& x, const int& y)
{
    MouseEvent e(x,y);
    auto ele = getElementByPosition(x, y);
    e.setRelativePosition(ele);
    if (hoverEle != ele) {
        if (hoverEle && !hoverEle->hittest(x,y)) {
            hoverEle->mouseLeave(e);
        }
        ele->mouseEnter(e);
    }
    ele->mouseMove(e);
    hoverEle = ele;
}

void WindowBase::windowMouseDown(const int& x, const int& y, const MouseButton& mouseBtn)
{
    MouseEvent e(x, y, mouseBtn);
    e.setRelativePosition(hoverEle);
    hoverEle->mouseDown(e);
}

void WindowBase::windowMouseUp(const int& x, const int& y, const MouseButton& mouseBtn)
{
    MouseEvent e(x, y, mouseBtn);
    e.setRelativePosition(hoverEle);
    hoverEle->mouseUp(e);
}

void WindowBase::paintArea()
{
    auto size = getWindowClientSize();
    auto w = size.w * scaleFactor;
    auto h = size.h * scaleFactor;
    auto pix = winImpl->getPix();

    PAINTSTRUCT ps;
    HDC hdc = BeginPaint(hwnd, &ps);
    HDC memDC = CreateCompatibleDC(hdc); //创建兼容的内存 DC 和位图
    // 创建与窗口 DC 兼容的位图 (尺寸使用逻辑尺寸，因为 StretchDIBits 会先缩放到 memDC)
    HBITMAP memBitmap = CreateCompatibleBitmap(hdc, size.w, size.h);
    HBITMAP oldBitmap = (HBITMAP)SelectObject(memDC, memBitmap);
    SetStretchBltMode(memDC, MAXSTRETCHBLTMODE);
    BITMAPINFO bmi = { 0 };
    bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    bmi.bmiHeader.biWidth = w;       // 源图像宽度 (物理像素)
    bmi.bmiHeader.biHeight = -h;    // 源图像高度 (负值表示 top-down)
    bmi.bmiHeader.biPlanes = 1;
    bmi.bmiHeader.biBitCount = 32;             // 32 bits per pixel
    bmi.bmiHeader.biCompression = BI_RGB;      // *** 注意颜色格式匹配问题 ***
    bmi.bmiHeader.biSizeImage = 0;             // 对 BI_RGB 可设为 0

    // 5. 使用 StretchDIBits 将 Skia Pixmap 缩放并绘制到内存 DC
    //    源: pix (physicalWidth x physicalHeight pixels at pix.addr())
    //    目标: memDC (logicSize.w x logicSize.h pixels)
    int stretchResult = StretchDIBits(
        memDC,                          // 目标 DC
        0, 0,                           // 目标 X, Y (逻辑像素)
        size.w, size.h,       // 目标宽度, 高度 (逻辑像素)
        0, 0,                           // 源 X, Y (物理像素)
        w, h,      // 源宽度, 高度 (物理像素)
        pix.addr(),                     // 源像素数据指针
        &bmi,                           // 源图像信息
        DIB_RGB_COLORS,                 // 颜色使用方式
        SRCCOPY                         // 光栅操作代码
    );
    // 6. 将内存 DC 的内容 (已缩放好的逻辑尺寸图像) BitBlt 到窗口 DC
    //    复制的区域大小是 StretchDIBits 绘制到 memDC 的区域大小 (逻辑像素)
    BitBlt(
        hdc,                            // 目标 DC (窗口 DC)
        0, 0,                           // 目标 X, Y
        size.w, size.h,       // 复制的宽度, 高度 (逻辑像素)
        memDC,                          // 源 DC (内存 DC)
        0, 0,                           // 源 X, Y
        SRCCOPY                         // 光栅操作代码
    );

    // 7. 清理 GDI 对象
    SelectObject(memDC, oldBitmap); // 恢复内存 DC 的旧位图
    DeleteObject(memBitmap);        // 删除我们创建的位图
    DeleteDC(memDC);                // 删除内存 DC
    EndPaint(hwnd, &ps);
    /*
    PAINTSTRUCT ps;
    HDC hdc = BeginPaint(hwnd, &ps);
    auto size = getSize();
    // 创建兼容DC和缓冲Bitmap
    HDC memDC = CreateCompatibleDC(hdc);
    HBITMAP memBitmap = CreateCompatibleBitmap(hdc, size.w, size.h);
    HBITMAP oldBitmap = (HBITMAP)SelectObject(memDC, memBitmap);
    BITMAPINFO bmi = { sizeof(BITMAPINFOHEADER), size.w, -size.h, 1, 32, BI_RGB, size.h * 4 * size.w, 0, 0, 0, 0 };

    // 获取需要绘制的窗口区域（以窗口为坐标系）
    RECT r = ps.rcPaint;
    int xDest = r.left;
    int yDest = r.top;
    int destW = r.right - r.left;
    int destH = r.bottom - r.top;

    // 同步源图像区域（注意 top-down 图像坐标）
    int xSrc = r.left;
    int ySrc = r.top;

    // Clip 防止越界
    xSrc = std::clamp(xSrc, 0, size.w);
    ySrc = std::clamp(ySrc, 0, size.h);
    destW = std::clamp(destW, 0, size.w - xSrc);
    destH = std::clamp(destH, 0, size.h - ySrc);

    auto pix = winImpl->getPix();
    // 仅绘制该区域
    StretchDIBits(memDC, xDest, yDest, destW, destH,    // 目标区域（窗口）
        xSrc, ySrc, destW, destH,      // 源区域（图像）
        pix.addr(),
        &bmi,
        DIB_RGB_COLORS,
        SRCCOPY);

    // 复制到窗口DC
    BitBlt(hdc, xDest, yDest, destW, destH, memDC, xDest, yDest, SRCCOPY);
    // 清理资源
    SelectObject(memDC, oldBitmap);
    DeleteObject(memBitmap);
    DeleteDC(memDC);
    EndPaint(hwnd, &ps);
    */
}



LRESULT CALLBACK WindowBase::customMsgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    return DefWindowProc(hwnd, msg, wParam, lParam);
}

const std::wstring& WindowBase::getWinClsName()
{
    static std::wstring clsName = [] {
        WNDCLASSEXW wcex;
        wcex.cbSize = sizeof(WNDCLASSEX);
        wcex.style = CS_HREDRAW | CS_VREDRAW;
        wcex.lpfnWndProc = &WindowBase::routeWindowMsg;
        wcex.cbClsExtra = 0;
        wcex.cbWndExtra = 0;
        wcex.hInstance = App::get()->hInstance;
        wcex.hIcon = LoadIcon(wcex.hInstance, (LPCTSTR)IDI_WINLOGO);
        wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
        wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
        wcex.lpszMenuName = nullptr;
        wcex.lpszClassName = L"Ling";
        wcex.hIconSm = LoadIcon(wcex.hInstance, (LPCTSTR)IDI_WINLOGO);
        RegisterClassExW(&wcex);
        return wcex.lpszClassName;
        }();
    return clsName;
}

void WindowBase::setScaleFactor()
{
    UINT dpi = GetDpiForWindow(hwnd);
    scaleFactor = dpi / 96.0f;
}

Element* WindowBase::getElementByPosition(int x, int y)
{
    Element* result = this;
    auto children = result->getChildren();
    while (children && children->size() > 0) {
        bool flag{ false };
        for (auto child : *children) //遍历子元素
        {
            if (child->hittest(x,y)) //命中测试
            {
                flag = true;
                result = child;
                children = result->getChildren();
                break; //结束本层级的遍历，开始遍历下一个层级，找到最底层的元素
            }
        }
        if (!flag) {
            break;//上级命中，但本级没有命中，直接退出循环。
        }
    }
    return result;
}

HWND WindowBase::getHandle()
{
    return hwnd;
}

float WindowBase::getScaleFactor()
{
    return scaleFactor;
}
