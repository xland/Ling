#include "D2D.h"
namespace Ling {
	static std::unique_ptr<D2D> d2d;
	D2D::D2D()
	{
		auto hr = DWriteCreateFactory(DWRITE_FACTORY_TYPE_ISOLATED, __uuidof(IDWriteFactory), reinterpret_cast<::IUnknown**>(dwriteFactory.GetAddressOf()));
		if (FAILED(hr)) {
			return;
		}
		hr = dwriteFactory->CreateTextFormat(L"Microsoft YaHei",nullptr,DWRITE_FONT_WEIGHT_NORMAL, DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL,
			12.f, L"zh-CN", baseTextFormat.GetAddressOf());
		if (FAILED(hr)) {
			return;
		}
		initIcon();
		ComPtr<ID3D11Device> d3dDevice;
		D3D_FEATURE_LEVEL featureLevels[]{ D3D_FEATURE_LEVEL_11_1, D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_0, D3D_FEATURE_LEVEL_10_1 };
		hr = D3D11CreateDevice(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, D3D11_CREATE_DEVICE_BGRA_SUPPORT, featureLevels, std::size(featureLevels),
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
	}
	void D2D::initIcon()
	{
		HRSRC hRes = FindResource(NULL, L"iconfont.ttf", RT_RCDATA);
		if (!hRes) {
			return;
		}
		HGLOBAL hData = LoadResource(NULL, hRes);
		if (!hData) {
			return;
		}
		void* pData = LockResource(hData);
		DWORD size = SizeofResource(NULL, hRes);
		ComPtr<IDWriteInMemoryFontFileLoader> loader;
		dwriteFactory->CreateInMemoryFontFileLoader(loader.GetAddressOf());
		dwriteFactory->RegisterFontFileLoader(loader.Get());
		ComPtr<IDWriteFontFile> fontFile;
		loader->CreateInMemoryFontFileReference(dwriteFactory.Get(), pData, size, nullptr, fontFile.GetAddressOf());
		ComPtr<IDWriteFontSetBuilder1> fontSetBuilder;
		dwriteFactory->CreateFontSetBuilder(fontSetBuilder.GetAddressOf());
		fontSetBuilder->AddFontFile(fontFile.Get());
		ComPtr<IDWriteFontSet> fontSet;
		fontSetBuilder->CreateFontSet(fontSet.GetAddressOf());
		ComPtr<IDWriteFontCollection1> fontCollection;
		dwriteFactory->CreateFontCollectionFromFontSet(fontSet.Get(), fontCollection.GetAddressOf());
		dwriteFactory->CreateTextFormat(L"icon", fontCollection.Get(), DWRITE_FONT_WEIGHT_NORMAL, DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL,
			12.f, L"", iconFormat.GetAddressOf());
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
	Composition::CompositionDrawingSurface D2D::createDrawingSurface(const Composition::Compositor& comp)
	{
		Composition::CompositionGraphicsDevice graphicsDevice{ nullptr };
		auto interop = comp.as<ABI::Windows::UI::Composition::ICompositorInterop>();
		interop->CreateGraphicsDevice(d2dDevice.Get(), reinterpret_cast<ABI::Windows::UI::Composition::ICompositionGraphicsDevice**>(winrt::put_abi(graphicsDevice)));
		return graphicsDevice.CreateDrawingSurface(
			winrt::Windows::Foundation::Size{ 0, 0 },
			winrt::Windows::Graphics::DirectX::DirectXPixelFormat::B8G8R8A8UIntNormalized,
			winrt::Windows::Graphics::DirectX::DirectXAlphaMode::Premultiplied
		);
	}
	ComPtr<IDWriteTextLayout> D2D::createTextLayout(const std::wstring& text, const float& w, const float& h)
	{
		ComPtr<IDWriteTextLayout> layout;
		dwriteFactory->CreateTextLayout(text.data(), text.length(), baseTextFormat.Get(), w, h, layout.GetAddressOf());
		return layout;
	}
}