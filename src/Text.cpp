#include "pch.h"
#include "../include/Text.h"
#include "../include/WinBase.h"
#include "../include/D2D.h"

namespace Ling {

    Text::Text(WinBase* win) : Node(win)
    {
        YGNodeSetContext(this->node, this);
        YGNodeSetMeasureFunc(this->node, &Text::measureCB);
    }

    Text::~Text()
    {
    }

    // 契约：measureFunc 必须是纯函数 —— 只读当前节点的度量信息并返回尺寸，
    // 不得修改 style、不得创建/销毁 Composition 资源。否则会在同一次
    // CalculateLayout 内触发 yoga 的重测量循环上限，导致后续兄弟节点的布局
    // 输出为“未完成的中间状态”，视觉上表现为最后一条节点被截掉一半。
    YGSize Text::measureCB(YGNodeConstRef node, float, YGMeasureMode, float, YGMeasureMode)
    {
        auto self = static_cast<Text*>(YGNodeGetContext(node));
        if (!self->textLayout) return { 0.f, 0.f };
        DWRITE_TEXT_METRICS m;
        self->textLayout->GetMetrics(&m);
        // 向上取整到整像素，返回给 yoga 用于布局（下游会得到整数 x/y）。
        return { std::ceil(m.width), std::ceil(m.height) };
    }

    void Text::setText(const std::wstring& text)
    {
        this->text = text;
        auto d2d = D2D::get();
        d2d->dwriteFactory->CreateTextLayout(text.data(), (UINT32)text.length(), d2d->baseTextFormat.Get(), FLT_MAX, FLT_MAX, textLayout.ReleaseAndGetAddressOf());
        textLayout->SetFontSize(fontSize * win->dpi, { 0, INT_MAX });
        if (!fontFamily.empty()) {
            textLayout->SetFontFamilyName(fontFamily.data(), { 0, INT_MAX });
        }
        YGNodeMarkDirty(node);
        win->refresh();
    }

    std::wstring Text::getText()
    {
        return text;
    }

    void Text::setFontSize(float val)
    {
        this->fontSize = val;
        if (textLayout.Get()) {
            textLayout->SetFontSize(fontSize * win->dpi, { 0, INT_MAX });
        }
        YGNodeMarkDirty(node);
        if (surface) paint();
    }

    void Text::setFontFamily(const std::wstring& val)
    {
        this->fontFamily = val;
        if (textLayout.Get() && !fontFamily.empty()) {
            textLayout->SetFontFamilyName(fontFamily.data(), { 0, INT_MAX });
        }
        YGNodeMarkDirty(node);
        if (surface) paint();
    }

    void Text::setColor(Color color)
    {
        if (this->color == color) return;
        this->color = color;
        if (surface) paint();
    }

    void Text::onDpiChanged()
    {
        if (textLayout.Get()) {
            textLayout->SetFontSize(fontSize * win->dpi, { 0, INT_MAX });
            YGNodeMarkDirty(node);
        }
    }

    void Text::layout()
    {
        Node::layout();
        paint();
    }

    void Text::paint()
    {
        // 此时 w/h 已经是 measureCB 返回的整数像素，一次到位创建/resize。
        const int pxW = static_cast<int>(w);
        const int pxH = static_cast<int>(h);
        if (pxW <= 0 || pxH <= 0) return;
        if (!surface) {
            auto d2d = D2D::get();
            surface = d2d->createDrawingSurface(win->compositor, (float)pxW, (float)pxH);
            auto brush = win->compositor.CreateSurfaceBrush(surface);
            visual.Brush(brush);
        }
        else {
            auto sz = surface.SizeInt32();
            if (sz.Width != pxW || sz.Height != pxH) {
                surface.Resize({ pxW, pxH });
            }
        }

        auto s = surface.as<ABI::Windows::UI::Composition::ICompositionDrawingSurfaceInterop>();
        ComPtr<ID2D1DeviceContext> ctx;
        POINT offset{};
        s->BeginDraw(nullptr, __uuidof(ID2D1DeviceContext), reinterpret_cast<void**>(ctx.GetAddressOf()), &offset);
        auto trans = D2D1::Matrix3x2F::Translation((float)offset.x, (float)offset.y);
        ctx->SetTransform(trans);
        ctx->Clear(0);    // surface 只画字，背景由外层容器负责
        ComPtr<ID2D1SolidColorBrush> brush;
        ctx->CreateSolidColorBrush(color.getD2DColor(), brush.GetAddressOf());
        // metric = surface 尺寸，从 (0,0) 起画即可
        ctx->DrawTextLayout({ 0.f, 0.f }, textLayout.Get(), brush.Get());
        s->EndDraw();
    }
}
