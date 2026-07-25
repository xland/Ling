#pragma once
#include <wrl.h>
#include <winrt/Windows.UI.Composition.h>
#include "Node.h"

namespace Ling {
	class WinBase;
	class Image :public Node
	{
	public:
		Image(WinBase* win);
		~Image();
		void paint();
		void loadImg(const std::wstring& imgPath);
	public:
		winrt::Windows::UI::Composition::CompositionDrawingSurface surface{ nullptr };
	private:
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
		static YGSize nodeMeasureCB(YGNodeConstRef node, float width, YGMeasureMode widthMode, float height, YGMeasureMode heightMode);
		void layout() override;
	private:
		Microsoft::WRL::ComPtr<ID2D1Bitmap> bitmap;
	};
}

