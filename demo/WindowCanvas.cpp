#include "pch.h"
#include "WindowCanvas.h"

WindowCanvas::WindowCanvas() : Ling::WinBase()
{
    setTitle(L"Canvas 自绘演示");
    setSize(800, 600);
    setCenter();
    onDestroy.add([this] { Ling::App::get()->quit(); });
    onSizeChanged.add([this] { redraw(); });
    createNativeWindow();
    redraw();
}

WindowCanvas::~WindowCanvas()
{
}

void WindowCanvas::onCreated()
{
    enableShadow();
    body->setBg(0xFFFFFFFF);
    body->setFlexDirection(Ling::FlexDirection::Column);
    titleBar = std::make_unique<TitleBar>(this);
    canvas = body->makeChild<Ling::Canvas>();
    canvas->setFlexGrow(1.f);      // 吃掉标题栏之外的所有空间
    // 节点已建，可以安全挂监听了；show() 会触发首次 WM_SIZE -> layout -> SizeChanged。
    show();
}

LRESULT WindowCanvas::onHitTest(const POINT pos)
{
    POINT pt = pos;
    ScreenToClient(hwnd, &pt);
    if (!isMaximized) {
        auto result = borderHitTest(pt);
        if (result != HTCLIENT) return result;
    }
    return titleBar->hitCaption(pt);
}

void WindowCanvas::redraw()
{
    if (!canvas) return;
    auto ctx = canvas->startPaint();
    if (!ctx) return;

    // 透明打底，避免上一帧的椭圆残留在缩小后的画布外。
    ctx->Clear(D2D1::ColorF(D2D1::ColorF::White));

    // 以画布中心为原点，留 10% 内边距，椭圆铺满剩余空间。
    const float cx = canvas->w * 0.5f;
    const float cy = canvas->h * 0.5f;
    const float rx = canvas->w * 0.4f;
    const float ry = canvas->h * 0.4f;
    D2D1_ELLIPSE ellipse{ { cx, cy }, rx, ry };

    Microsoft::WRL::ComPtr<ID2D1SolidColorBrush> fill;
    ctx->CreateSolidColorBrush(Ling::Color(0x6699FFFF).getD2DColor(), fill.GetAddressOf());
    ctx->FillEllipse(ellipse, fill.Get());

    Microsoft::WRL::ComPtr<ID2D1SolidColorBrush> stroke;
    ctx->CreateSolidColorBrush(Ling::Color(0xFF9922FF).getD2DColor(), stroke.GetAddressOf());
    ctx->DrawEllipse(ellipse, stroke.Get(), 2.f * dpi);

    canvas->finishPaint();
}
