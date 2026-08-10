#pragma once
#include <d2d1_1.h>
#include <dxgi1_2.h>
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
    //
    // 两种后端，用 enableSwapChain() 选择，绘制代码完全一样：
    //   - 默认 CompositionDrawingSurface：单缓冲，省一块纹理，适合内容很少变的画布。
    //   - swap chain：双缓冲 + Present 整帧原子换出，适合每帧重画的画布。
    class Canvas : public Node
    {
    public:
        Canvas(WinBase* win);
        ~Canvas();

        // 换成 swap chain 后端。必须在第一次 startPaint() 之前调用，之后调用无效。
        //
        // 为什么需要它：CompositionDrawingSurface 是单缓冲的 —— BeginDraw 拿到的就是合成器
        // 正在读的那张纹理，EndDraw 也不保证这一批绘制原子呈现。所以"Clear 擦干净 → 逐个重画"
        // 这段中间态可能刚好被合成器采样到，高频重绘时就是整帧闪烁（典型表现：只剩背景，
        // 前景内容集体消失一帧）。swap chain 画在后台缓冲上，Present 时才换到前台，没有中间态。
        //
        // 代价是多一块与画布等大的纹理（双缓冲），所以只给真正需要的画布用。
        void enableSwapChain();

        // 开始一次绘制。
        //   - 返回的指针在 finishPaint() 之前有效；finishPaint 之后再用即 UB。
        //   - 坐标系原点即画布左上角，直接从 (0,0) 画到 (w,h)。
        //   - Canvas 不清屏；如需透明背景请自行 ctx->Clear(0)，需要底色请 Clear(Color(...).getD2DColor())。
        //     swap chain 后端的后台缓冲里是上一轮的旧内容，务必自己清屏或整面覆盖。
        //   - 尺寸未就绪（还没走过 layout 或尺寸为 0）时返回 nullptr。
        ID2D1DeviceContext* startPaint();

        // 结束绘制并提交（swap chain 后端在这里 Present）；未开始过绘制时调用是安全的空操作。
        void finishPaint();

    public:
        // 当前 D2D surface 尺寸（= yoga 布局后的 w/h，物理像素）。
        // 使用者可用于计算 DrawRectangle 之类的坐标。
        // 注：swap chain 后端下这个字段为空，尺寸直接看 Node 的 w/h。
        winrt::Windows::UI::Composition::CompositionDrawingSurface surface{ nullptr };

    private:
        // 按 pxW/pxH 准备 swap chain 及其后台缓冲位图，尺寸没变就复用。返回 false 表示这一帧画不了。
        bool ensureSwapChain(UINT pxW, UINT pxH);

    private:
        Microsoft::WRL::ComPtr<ID2D1DeviceContext> ctx;
        // 以下三个只在 swap chain 后端下有值。ctx 会长期持有 targetBmp 作为绘制目标，
        // 所以这条路径上的 ctx 是 Canvas 自己创建的独立 context，不与别处共用。
        Microsoft::WRL::ComPtr<IDXGISwapChain1> swap;
        Microsoft::WRL::ComPtr<ID2D1Bitmap1> targetBmp;
        bool useSwapChain{ false };
        bool painting{ false };
    };
}
