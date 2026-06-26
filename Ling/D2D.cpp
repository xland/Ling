#include "D2D.h"
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
		// 4. 加载资源中的字体
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
		dwriteFactory->CreateTextLayout(text.data(), (UINT32)(text.length()), baseTextFormat.Get(), w, h, layout.GetAddressOf());
		return layout;
	}
}