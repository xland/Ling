#pragma once
#include <string>
#include "Node.h"

namespace Ling {
    class WinBase;
    class Image;

    // 复合组件：外层是一个标准 flex 容器（支持 flexGrow/setBg/对齐 等所有 Node 特性），
    // 内层是一个私有的 Image 节点，负责实际的位图绘制。
    // 外层容器决定"图像画在哪里"（justify / align 完成水平/垂直居中）；
    // 内层 Image 决定"图像画多大" —— 默认取原生像素尺寸，若超出容器约束则等比收缩。
    class ImageBox : public Node
    {
    public:
        ImageBox(WinBase* win);
        ~ImageBox();
        void loadImg(const std::wstring& path);
    private:
        Image* img{ nullptr };
    };
}
