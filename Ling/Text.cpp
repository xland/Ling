#include "Text.h"
namespace Ling {
	static ComPtr<IDWriteFactory5> dwriteFactory;
	static ComPtr<IDWriteTextFormat> baseTextFormat;


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
		
		auto surface = comp.CreateDrawingSurface(
			winrt::Windows::Foundation::Size{ 0, 0 },
			winrt::Windows::Graphics::DirectX::DirectXPixelFormat::B8G8R8A8UIntNormalized,
			winrt::Windows::Graphics::DirectX::DirectXAlphaMode::Premultiplied
		);
		//m_brush = compositor.CreateSurfaceBrush(m_surface);
		//// 3. 将画刷赋给基类的 SpriteVisual
		//m_visual.Brush(m_brush);
	}
}