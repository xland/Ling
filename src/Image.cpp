#include "pch.h"
#include <wincodec.h>
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

	void Image::loadImg(const std::wstring& imgPath)
	{
		auto d2d = D2D::get();
		ComPtr<IWICImagingFactory> wicFactory;
		auto hr = CoCreateInstance(CLSID_WICImagingFactory, NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&wicFactory));
		if (FAILED(hr)) {
			return;
		}
		ComPtr<IWICBitmapDecoder> decoder;
		// 不指定特定解码器，让 WIC 自动匹配
		hr = wicFactory->CreateDecoderFromFilename(imgPath.data(), nullptr,GENERIC_READ,WICDecodeMetadataCacheOnLoad,&decoder);
		if (FAILED(hr)) {
			return;
		}
		ComPtr<IWICBitmapFrameDecode> frame = nullptr;
		hr = decoder->GetFrame(0, &frame);
		if (FAILED(hr)) {
			return;
		}
		ComPtr<IWICFormatConverter> converter = nullptr;
		hr = wicFactory->CreateFormatConverter(&converter);
		if (FAILED(hr)) {
			return;
		}
		hr = converter->Initialize(frame.Get(),GUID_WICPixelFormat32bppPBGRA,WICBitmapDitherTypeNone,NULL,0.f,WICBitmapPaletteTypeMedianCut);
		if (FAILED(hr)) {
			return;
		}
		hr = d2d->deviceContext->CreateBitmapFromWicBitmap(converter.Get(),nullptr, bitmap.GetAddressOf());
		if (FAILED(hr)) {
			return;
		}
		YGNodeMarkDirty(node);
	}


	YGSize Image::nodeMeasureCB(YGNodeConstRef node, float width, YGMeasureMode widthMode, float height, YGMeasureMode heightMode)
	{
		//如果你在 YGNodeStyleSetWidth(node, 100) / YGNodeStyleSetHeight(node, 50) 里已经指定了固定大小，Yoga 就直接用这个值，不会去调用 measureFunc。
		//如果父容器已经约束住了大小，比如 flex : 1 填充满了，Yoga 也不会再问 measureFunc。
		auto self = static_cast<Image*>(YGNodeGetContext(node));
		if (!self->bitmap) return { 0.f, 0.f };
		auto size = self->bitmap->GetSize();
		return { std::ceil(size.width), std::ceil(size.height) };
	}

	void Image::paint()
	{
		// 用 bitmap 的原生像素尺寸,不再看 yoga 的 w/h
		auto px = bitmap->GetPixelSize();
		const int pxW = static_cast<int>(px.width);
		const int pxH = static_cast<int>(px.height);
		if (pxW <= 0 || pxH <= 0) return;

		if (!surface) {
			auto d2d = D2D::get();
			surface = d2d->createDrawingSurface(win->compositor, (float)pxW, (float)pxH);
			auto brush = win->compositor.CreateSurfaceBrush(surface);
			brush.Stretch(winrt::Windows::UI::Composition::CompositionStretch::None);  // 关键:不让 Composition 再缩
			// 想要"图像画在 visual 中央"就取消下面这句注释:
			 //brush.HorizontalAlignmentRatio(0.5f); brush.VerticalAlignmentRatio(0.5f);
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
		ctx->SetTransform(D2D1::Matrix3x2F::Translation((float)offset.x, (float)offset.y));
		ctx->Clear(0);
		D2D1_RECT_F dstRect{ 0.f, 0.f, (float)pxW, (float)pxH };
		ctx->DrawBitmap(bitmap.Get(), dstRect, 1.0f, D2D1_BITMAP_INTERPOLATION_MODE_LINEAR);
		s->EndDraw();
	}

	void Image::layout()
	{
		Node::layout();
		paint();
	}
}
