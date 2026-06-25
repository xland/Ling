#include "Text.h"
namespace Ling {
	static ComPtr<IDWriteFactory5> dwriteFactory;
	static ComPtr<IDWriteTextFormat> baseTextFormat;
	static ComPtr<ID2D1Device> d2dDevice;
	static ComPtr<ID2D1Factory1> d2dFactory;
	Text::Text()
	{
		if (!dwriteFactory.Get()) {
			auto hr = DWriteCreateFactory(DWRITE_FACTORY_TYPE_ISOLATED, __uuidof(IDWriteFactory), reinterpret_cast<::IUnknown**>(dwriteFactory.GetAddressOf()));
			if (FAILED(hr)) {

			}
		}
		if (!baseTextFormat.Get()) {
			auto hr = dwriteFactory->CreateTextFormat(L"Microsoft YaHei", nullptr, DWRITE_FONT_WEIGHT_NORMAL, DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL,
				12.f, L"zh-CN", baseTextFormat.GetAddressOf());
			if (FAILED(hr)) {

			}
		}
		if (!d2dDevice.Get()) {
			ComPtr<ID3D11Device> d3dDevice;
			D3D_FEATURE_LEVEL featureLevels[] { D3D_FEATURE_LEVEL_11_1, D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_0, D3D_FEATURE_LEVEL_10_1 };
			D3D11CreateDevice(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, D3D11_CREATE_DEVICE_BGRA_SUPPORT, featureLevels, std::size(featureLevels), 
				D3D11_SDK_VERSION, d3dDevice.GetAddressOf(), nullptr, nullptr);			
			D2D1_FACTORY_OPTIONS options{};
			D2D1CreateFactory( D2D1_FACTORY_TYPE_SINGLE_THREADED, IID_ID2D1Factory1, &options, (void**)d2dFactory.GetAddressOf());
			ComPtr<IDXGIDevice> dxgiDevice;
			d3dDevice->QueryInterface(dxgiDevice.GetAddressOf());
			d2dFactory->CreateDevice(dxgiDevice.Get(), d2dDevice.GetAddressOf());
		}
	}

	Text::~Text()
	{
	}

	void Text::setText(const std::wstring text)
	{
		this->text = text;
	}

	void Text::initProperty(const Composition::Compositor& comp)
	{
		visual = comp.CreateSpriteVisual();
		win = parent->win;
		Composition::CompositionGraphicsDevice graphicsDevice{ nullptr };
		auto interop = comp.as<ABI::Windows::UI::Composition::ICompositorInterop>();
		interop->CreateGraphicsDevice(d2dDevice.Get(),reinterpret_cast<ABI::Windows::UI::Composition::ICompositionGraphicsDevice**>(winrt::put_abi(graphicsDevice)));
		surface = graphicsDevice.CreateDrawingSurface(
			winrt::Windows::Foundation::Size{ 0, 0 },
			winrt::Windows::Graphics::DirectX::DirectXPixelFormat::B8G8R8A8UIntNormalized,
			winrt::Windows::Graphics::DirectX::DirectXAlphaMode::Premultiplied
		);
		brush = comp.CreateSurfaceBrush(surface);
		visual.Brush(brush);
		dwriteFactory->CreateTextLayout(text.data(),text.length(),baseTextFormat.Get(), FLT_MAX, FLT_MAX, textLayout.GetAddressOf());
		YGNodeSetMeasureFunc(node, &Text::nodeMeasureCB);
	}
	void Text::layout()
	{
		x = YGNodeLayoutGetLeft(node);
		y = YGNodeLayoutGetTop(node);
		xAbs = parent->xAbs + x;
		yAbs = parent->yAbs + y;
		w = YGNodeLayoutGetWidth(node);
		h = YGNodeLayoutGetHeight(node);
		visual.Offset({ x, y, 0.0f });
		visual.Size({ w, h });
		surface.Resize({(int)w,(int)h});
		auto surfaceInterop = surface.as<ABI::Windows::UI::Composition::ICompositionDrawingSurfaceInterop>();
		ComPtr<ID2D1DeviceContext> d2dContext;
		POINT offset{};
		HRESULT hr = surfaceInterop->BeginDraw(nullptr,__uuidof(ID2D1DeviceContext),reinterpret_cast<void**>(d2dContext.GetAddressOf()),&offset);
		auto trans = D2D1::Matrix3x2F::Translation(static_cast<float>(offset.x), static_cast<float>(offset.y));
		d2dContext->SetTransform(trans);
		d2dContext->Clear(0);
		ComPtr<ID2D1SolidColorBrush> brush;
		d2dContext->CreateSolidColorBrush(D2D1::ColorF(0x000000), brush.GetAddressOf());
		d2dContext->DrawTextLayout({ 0, 0 }, textLayout.Get(), brush.Get());
		d2dContext->EndDraw();
	}
	YGSize Text::nodeMeasureCB(YGNodeConstRef node, float width, YGMeasureMode widthMode, float height, YGMeasureMode heightMode)
	{
		auto self = static_cast<Text*>(YGNodeGetContext(node));
		float maxWidth = (widthMode == YGMeasureModeUndefined) ? 0.0f : width;
		DWRITE_TEXT_METRICS metrics;
		self->textLayout->GetMetrics(&metrics);
		return { metrics.width, metrics.height };
	}
}