#include "pch.h"
#include "../include/Image.h"
#include "../include/WinBase.h"
#include "../include/D2D.h"

namespace Ling {

	Image::Image(WinBase* win) :Node(win)
	{
		YGNodeSetContext(this->node, this);
		YGNodeSetMeasureFunc(this->node, &Image::nodeMeasureCB);
	}

	Image::~Image()
	{
	}

	YGSize Image::nodeMeasureCB(YGNodeConstRef node, float width, YGMeasureMode widthMode, float height, YGMeasureMode heightMode)
	{
		//如果你在 YGNodeStyleSetWidth(node, 100) / YGNodeStyleSetHeight(node, 50) 里已经指定了固定大小，Yoga 就直接用这个值，不会去调用 measureFunc。
		//如果父容器已经约束住了大小，比如 flex : 1 填充满了，Yoga 也不会再问 measureFunc。
		auto self = static_cast<Image*>(YGNodeGetContext(node));
		
		return { 0, 0 };
	}

	void Image::paint()
	{
		auto s = surface.as<ABI::Windows::UI::Composition::ICompositionDrawingSurfaceInterop>();
		ComPtr<ID2D1DeviceContext> ctx;
		POINT offset{};   // 物理像素
		HRESULT hr = s->BeginDraw(nullptr, __uuidof(ID2D1DeviceContext), reinterpret_cast<void**>(ctx.GetAddressOf()), &offset);
		// 全程使用物理像素，不调 SetDpi
		auto trans = D2D1::Matrix3x2F::Translation(static_cast<float>(offset.x), static_cast<float>(offset.y));
		ctx->SetTransform(trans);
		ctx->Clear(0);
		s->EndDraw();
	}

	void Image::layout()
	{
		Node::layout();
		paint();
	}
}
