#pragma once
#include <d2d1_1.h>
#include <wrl.h>
#include <winrt/Windows.UI.Composition.h>
#include "Node.h"

namespace Ling {
    class WinBase;

    // 使用者自绘的画布节点。尺寸由外部 flex / setSize 决定；Canvas 只负责在这块矩形上
    // 拉起一个 D2D surface，把 ID2D1DeviceContext 交给使用者，让使用者调 D2D API 自己画。
    //
    // 生命周期：startPaint() 返回可用的 ID2D1DeviceContext*，绘制完成后必须成对调用
    // finishPaint()。未 finishPaint 之前 surface 处于 BeginDraw 状态，同设备上再次
    // BeginDraw 会失败。禁止嵌套 / 跨线程使用。
    class Canvas : public Node
    {
    public:
        Canvas(WinBase* win);
        ~Canvas();

        // 开始一次绘制。
        //   - 返回的指针在 finishPaint() 之前有效；finishPaint 之后再用即 UB。
        //   - 已为你 SetTransform 过 surface 的 tile offset：从 (0,0) 起画到 (w,h) 即可。
        //   - Canvas 不清屏；如需透明背景请自行 ctx->Clear(0)，需要底色请 Clear(Color(...).getD2DColor())。
        //   - 尺寸未就绪（还没走过 layout 或尺寸为 0）时返回 nullptr。
        ID2D1DeviceContext* startPaint();

        // 结束绘制并释放 ctx；未开始过绘制时调用是安全的空操作。
        void finishPaint();

    public:
        // 当前 D2D surface 尺寸（= yoga 布局后的 w/h，物理像素）。
        // 使用者可用于计算 DrawRectangle 之类的坐标。
        winrt::Windows::UI::Composition::CompositionDrawingSurface surface{ nullptr };

    private:
        Microsoft::WRL::ComPtr<ID2D1DeviceContext> ctx;
        bool painting{ false };
    };
}
