#pragma once
#include <wrl.h>
#include <dwrite_3.h>
#include <winrt/Windows.UI.Composition.h>
#include "../include/Node.h"

namespace Ling {
    class WinBase;

    // 内部实现类：一段文本的最小渲染单元。
    //   - surface 尺寸恒等于 DWrite 的 metric（避免父容器 flexGrow 撑爆显存）；
    //   - 通过 yoga 的 measureFunc 把 metric 报告给父节点，父节点用原生 flex
    //     完成对齐（不再在 paint() 里手写偏移）。
    // 只在 src 内部使用，不对外暴露。
    class Text : public Node
    {
    public:
        Text(WinBase* win);
        ~Text();
        void setText(const std::wstring& text);
        std::wstring getText();
        void setFontSize(float val);
        void setFontFamily(const std::wstring& val);
        void setColor(Color color);
    private:
        // 尺寸/布局完全由内容决定，屏蔽掉外部改尺寸、改子节点的 API，避免误用。
        using Node::makeChild;
        using Node::setFlexGrow;
        using Node::setFlexShrink;
        using Node::setWidth;
        using Node::setHeight;
        using Node::setSize;
        using Node::setWidthPercent;
        using Node::setHeightPercent;
        using Node::setSizePercent;
        using Node::setPadding;
        using Node::setPaddingLeft;
        using Node::setPaddingTop;
        using Node::setPaddingRight;
        using Node::setPaddingBottom;
        using Node::setFlexWrap;
        using Node::setAlignItems;
        using Node::setJustifyContent;
        using Node::setFlexDirection;
        using Node::setBg;

        static YGSize measureCB(YGNodeConstRef node, float width, YGMeasureMode widthMode, float height, YGMeasureMode heightMode);
        void layout() override;
        void onDpiChanged() override;
        void paint();

    private:
        winrt::Windows::UI::Composition::CompositionDrawingSurface surface{ nullptr };
        Microsoft::WRL::ComPtr<IDWriteTextLayout> textLayout;
        Color color{ 0x333333FF };
        std::wstring text;
        std::wstring fontFamily;
        float fontSize{ 12.f };
    };
}
