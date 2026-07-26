

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
    void D2D::init()
    {
        d2d.reset(new D2D());
    }
    D2D::~D2D()
    {
    }
    D2D* D2D::get()
    {
        return d2d.get();
    }

	void D2D::initDevice()
	{
		ComPtr<ID3D11Device> d3dDevice;
		D3D_FEATURE_LEVEL levels[]{ D3D_FEATURE_LEVEL_11_1, D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_0, D3D_FEATURE_LEVEL_10_1 };
		auto count = std::size(levels);
		auto hr = D3D11CreateDevice(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, D3D11_CREATE_DEVICE_BGRA_SUPPORT, levels, count, D3D11_SDK_VERSION, &d3dDevice, nullptr, nullptr);
		if (FAILED(hr)) {
			_ASSERT_EXPR(FALSE, L"D3D11CreateDevice，error");
			return;
		}
		D2D1_FACTORY_OPTIONS options{};
		hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, IID_ID2D1Factory1, &options, (void**)&d2dFactory);
		if (FAILED(hr)) {
			_ASSERT_EXPR(FALSE, L"D2D1CreateFactory，error");
			return;
		}
		ComPtr<IDXGIDevice> dxgiDevice;
		d3dDevice->QueryInterface(dxgiDevice.ReleaseAndGetAddressOf());
		hr = d2dFactory->CreateDevice(dxgiDevice.Get(), d2dDevice.ReleaseAndGetAddressOf());
		if (FAILED(hr)) {
			_ASSERT_EXPR(FALSE, L"D2D1CreateFactory CreateDevice，error");
			return;
		}
		hr = d2dDevice->CreateDeviceContext(D2D1_DEVICE_CONTEXT_OPTIONS_NONE, deviceContext.ReleaseAndGetAddressOf());
	}
    // 1. 初始化：仅加载系统字体（默认行为）
    void D2D::initFont()
    {
        auto hr = DWriteCreateFactory(DWRITE_FACTORY_TYPE_ISOLATED,__uuidof(IDWriteFactory5),reinterpret_cast<::IUnknown**>(dwriteFactory.ReleaseAndGetAddressOf()));
        if (FAILED(hr)) {
            _ASSERT_EXPR(FALSE, L"DWriteCreateFactory, error");
            return;
        }
        // 获取系统字体集
        ComPtr<IDWriteFontCollection> sysCollection;
        dwriteFactory->GetSystemFontCollection(&sysCollection);
        ComPtr<IDWriteFontCollection1> sysCollection1;
        sysCollection.As(&sysCollection1);
        ComPtr<IDWriteFontSet> sysFontSet;
        sysCollection1->GetFontSet(&sysFontSet);

        // 创建 FontSetBuilder 并仅加入系统字体
        ComPtr<IDWriteFontSetBuilder1> builder;
        dwriteFactory->CreateFontSetBuilder(&builder);
        builder->AddFontSet(sysFontSet.Get());

        // 生成字体集合
        ComPtr<IDWriteFontSet> combinedFontSet;
        builder->CreateFontSet(&combinedFontSet);
        dwriteFactory->CreateFontCollectionFromFontSet(combinedFontSet.Get(), &fontCollection);

        // 创建默认 TextFormat
        hr = dwriteFactory->CreateTextFormat(L"Microsoft YaHei", fontCollection.Get(),
            DWRITE_FONT_WEIGHT_NORMAL, DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL,
            12.f, L"zh-CN", baseTextFormat.GetAddressOf());
    }

    // 2. 附加：在已有字体集基础上追加自定义字体
    void D2D::addFonts(const std::vector<std::wstring>& fontResourceNames)
    {
        if (fontResourceNames.empty()) return;
        // 获取当前字体集作为基础
        ComPtr<IDWriteFontCollection1> currentCollection1;
        fontCollection.As(&currentCollection1);
        ComPtr<IDWriteFontSet> currentFontSet;
        currentCollection1->GetFontSet(&currentFontSet);
        // 创建新的 Builder，以当前字体集为基底
        ComPtr<IDWriteFontSetBuilder1> builder;
        dwriteFactory->CreateFontSetBuilder(&builder);
        builder->AddFontSet(currentFontSet.Get());
        // 创建内存字体加载器
        ComPtr<IDWriteInMemoryFontFileLoader> loader;
        dwriteFactory->CreateInMemoryFontFileLoader(loader.GetAddressOf());
        dwriteFactory->RegisterFontFileLoader(loader.Get());
        // 逐个加载并追加自定义字体
        for (const auto& resName : fontResourceNames) {
            auto [pData, size] = Util::getRes(resName.c_str());
            if (pData == nullptr || size == 0) {
                _ASSERT_EXPR(FALSE, L"load font res, error");
                return;
            }
            ComPtr<IDWriteFontFile> fontFile;
            auto hr = loader->CreateInMemoryFontFileReference(dwriteFactory.Get(), pData, size, nullptr, fontFile.GetAddressOf());
            if (FAILED(hr)) {
                _ASSERT_EXPR(FALSE, L"font res decode, error");
                continue;
            }
            builder->AddFontFile(fontFile.Get());
        }
        // 重新生成合并后的字体集合
        ComPtr<IDWriteFontSet> combinedFontSet;
        builder->CreateFontSet(&combinedFontSet);
        dwriteFactory->CreateFontCollectionFromFontSet(combinedFontSet.Get(), &fontCollection);

        // 重新创建 TextFormat（字体集已变更，需要重建）
        dwriteFactory->CreateTextFormat(L"Microsoft YaHei", fontCollection.Get(),
            DWRITE_FONT_WEIGHT_NORMAL, DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL,
            12.f, L"zh-CN", baseTextFormat.ReleaseAndGetAddressOf());
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

}
