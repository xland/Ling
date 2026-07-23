

#include "pch.h"
#include "../include/D2D.h"
#include "../include/Util.h"
#include <wincodec.h>
namespace Ling {
	static std::unique_ptr<D2D> d2d;
	D2D::D2D()
	{
		initDevice();
		initFont();
	}
	void D2D::initFont()
	{
		auto hr = DWriteCreateFactory(DWRITE_FACTORY_TYPE_ISOLATED, __uuidof(IDWriteFactory), reinterpret_cast<::IUnknown**>(dwriteFactory.GetAddressOf()));
		if (FAILED(hr)) {
			return;
		}
		ComPtr<IDWriteFactory5> factory5;
		dwriteFactory.As(&factory5);
		// 1. 获取系统字体集
		ComPtr<IDWriteFontCollection> sysCollection;
		dwriteFactory->GetSystemFontCollection(&sysCollection);
		ComPtr<IDWriteFontCollection1> sysCollection1;
		sysCollection.As(&sysCollection1);
		ComPtr<IDWriteFontSet> sysFontSet;
		sysCollection1->GetFontSet(&sysFontSet);
		// 2. 创建 FontSetBuilder
		ComPtr<IDWriteFontSetBuilder1> builder;
		factory5->CreateFontSetBuilder(&builder);
		// 3. 将系统字体加入 Builder
		builder->AddFontSet(sysFontSet.Get());
		auto [pData, size] = Util::getRes(L"iconfont.ttf");
		ComPtr<IDWriteInMemoryFontFileLoader> loader;
		dwriteFactory->CreateInMemoryFontFileLoader(loader.GetAddressOf());
		dwriteFactory->RegisterFontFileLoader(loader.Get());
		ComPtr<IDWriteFontFile> fontFile;
		hr = loader->CreateInMemoryFontFileReference(dwriteFactory.Get(), pData, size, nullptr, fontFile.GetAddressOf());
		if (FAILED(hr)) {
			return;
		}
		builder->AddFontFile(fontFile.Get());
		// 5. 生成新的合并字体集和集合
		ComPtr<IDWriteFontSet> combinedFontSet;
		builder->CreateFontSet(&combinedFontSet);
		factory5->CreateFontCollectionFromFontSet(combinedFontSet.Get(), &fontCollection);
		// 下面两句就都能成功找到字体了！
		hr = dwriteFactory->CreateTextFormat(L"Microsoft YaHei", fontCollection.Get(),
			DWRITE_FONT_WEIGHT_NORMAL, DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL,
			12.f, L"zh-CN", baseTextFormat.GetAddressOf());
	}
	void D2D::initDevice()
	{
		ComPtr<ID3D11Device> d3dDevice;
		D3D_FEATURE_LEVEL featureLevels[]{ D3D_FEATURE_LEVEL_11_1, D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_0, D3D_FEATURE_LEVEL_10_1 };
		auto hr = D3D11CreateDevice(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, D3D11_CREATE_DEVICE_BGRA_SUPPORT, featureLevels, std::size(featureLevels),
			D3D11_SDK_VERSION, d3dDevice.GetAddressOf(), nullptr, nullptr);
		if (FAILED(hr)) {
			return;
		}
		D2D1_FACTORY_OPTIONS options{};
		hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, IID_ID2D1Factory1, &options, (void**)d2dFactory.GetAddressOf());
		if (FAILED(hr)) {
			return;
		}
		ComPtr<IDXGIDevice> dxgiDevice;
		d3dDevice->QueryInterface(dxgiDevice.GetAddressOf());
		hr = d2dFactory->CreateDevice(dxgiDevice.Get(), d2dDevice.GetAddressOf());
		if (FAILED(hr)) {
			return;
		}
		hr = d2dDevice->CreateDeviceContext(D2D1_DEVICE_CONTEXT_OPTIONS_NONE, deviceContext.GetAddressOf());
	}
	D2D::~D2D()
	{
	}
	D2D* D2D::get()
	{
		if (!d2d.get()) {
			d2d.reset(new D2D());
		}
		return d2d.get();
	}
	ID2D1Factory1* D2D::getFactory()
	{
		return d2d->d2dFactory.Get();
	}
	Composition::CompositionDrawingSurface D2D::createDrawingSurface(const Composition::Compositor& comp, float w, float h)
	{
		Composition::CompositionGraphicsDevice graphicsDevice{ nullptr };
		auto interop = comp.as<ABI::Windows::UI::Composition::ICompositorInterop>();
		interop->CreateGraphicsDevice(d2dDevice.Get(), reinterpret_cast<ABI::Windows::UI::Composition::ICompositionGraphicsDevice**>(winrt::put_abi(graphicsDevice)));
		return graphicsDevice.CreateDrawingSurface(
			winrt::Windows::Foundation::Size{ w, h },
			winrt::Windows::Graphics::DirectX::DirectXPixelFormat::B8G8R8A8UIntNormalized,
			winrt::Windows::Graphics::DirectX::DirectXAlphaMode::Premultiplied
		);
	}
	ComPtr<IDWriteTextLayout> D2D::createTextLayout(const std::wstring& text, float w, float h)
	{
		ComPtr<IDWriteTextLayout> layout;
		dwriteFactory->CreateTextLayout(text.data(), (UINT32)(text.length()), baseTextFormat.Get(), w, h, layout.GetAddressOf());
		return layout;
	}

	void D2D::setEllipsis(IDWriteTextLayout* layout, float maxW, float maxH)
	{
		// 单行排布，让 trimming 直接在 maxW 处按字符下刀；否则默认 WRAP 会按单词换行，
		// 即使 granularity=CHARACTER 也只能在单词边界截断。
		layout->SetWordWrapping(DWRITE_WORD_WRAPPING_NO_WRAP);
		DWRITE_TRIMMING trimming = {};
		trimming.granularity = DWRITE_TRIMMING_GRANULARITY_CHARACTER;
		ComPtr<IDWriteInlineObject> pInlineObject;
		HRESULT hr = dwriteFactory->CreateEllipsisTrimmingSign(layout, &pInlineObject);
		hr = layout->SetTrimming(&trimming, pInlineObject.Get());
		layout->SetMaxWidth(maxW);
		layout->SetMaxHeight(maxH);
	}

	ComPtr<ID2D1Bitmap> D2D::createBitmap(const std::wstring& imgPath)
	{
		ComPtr<IWICImagingFactory> wicFactory;
		auto hr = CoCreateInstance(
			CLSID_WICImagingFactory,
			NULL,
			CLSCTX_INPROC_SERVER,
			IID_PPV_ARGS(&wicFactory)
		);
		ComPtr<IWICBitmapDecoder> decoder;
		hr = wicFactory->CreateDecoderFromFilename(imgPath.data(), nullptr,// 不指定特定解码器，让 WIC 自动匹配
			GENERIC_READ,                     // 读取权限
			WICDecodeMetadataCacheOnLoad,     // 加载时缓存元数据
			&decoder
		);
		ComPtr<IWICBitmapFrameDecode> frame = nullptr;
		hr = decoder->GetFrame(0, &frame);

		ComPtr<IWICFormatConverter> converter = nullptr;
		hr = wicFactory->CreateFormatConverter(&converter);
		hr = converter->Initialize(
			frame.Get(),
			GUID_WICPixelFormat32bppPBGRA,
			WICBitmapDitherTypeNone,
			NULL,
			0.f,
			WICBitmapPaletteTypeMedianCut
		);
		ComPtr<ID2D1Bitmap> bitmap;
		hr = deviceContext->CreateBitmapFromWicBitmap(
			converter.Get(),
			nullptr, // 使用默认位图属性
			&bitmap
		);
		return bitmap;
	}

	ComPtr<ID2D1PathGeometry> D2D::createPath(const std::vector<float>& points)
	{
		ComPtr<ID2D1PathGeometry> path;
		d2dFactory->CreatePathGeometry(path.GetAddressOf());
		ComPtr<ID2D1GeometrySink> sink;
		path->Open(sink.GetAddressOf());
		sink->BeginFigure({ points[0], points[1] }, D2D1_FIGURE_BEGIN_FILLED);
		sink->AddLine({ points[2], points[3] });
		sink->AddLine({ points[4], points[5] });
		sink->AddLine({ points[6], points[7] });
		sink->EndFigure(D2D1_FIGURE_END_CLOSED);
		sink->Close();
		return path;
	}

}
