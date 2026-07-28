#pragma once
#include <string>
#include "Node.h"
#include "Color.h"

namespace Ling {
    class WinBase;
    class Text;   // 实现在 src/Text.h，Label 只持有指针，头文件无需可见

    // 复合组件：外层是一个标准 flex 容器（支持 flexGrow/setBg/对齐 等所有 Node 特性），
    // 内层是一个私有的 Text 节点，负责实际的文本绘制。
    // surface 尺寸只随文字 metric 走，与容器尺寸无关。
    class Label : public Node
    {
    public:
        Label(WinBase* win);
        ~Label();
        std::wstring getText();
        void setText(const std::wstring& text);
        void setFontSize(float val);
        void setFontFamily(const std::wstring& val);
        void setColor(Color color);
    private:
        Text* text{ nullptr };
    };
}
