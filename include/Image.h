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
		using Node::setAlignItems;
		using Node::setJustifyContent;
		using Node::setFlexDirection;
		static YGSize nodeMeasureCB(YGNodeConstRef node, float width, YGMeasureMode widthMode, float height, YGMeasureMode heightMode);
		void layout() override;
	private:
		ComPtr<ID2D1Bitmap> bitmap;
	};
}

