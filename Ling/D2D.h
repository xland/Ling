#pragma once
#include <string>
#include <wrl.h>
#include <d2d1_1.h>
#include <dwrite_3.h>
#include <winrt/Windows.UI.Composition.h>
using namespace Microsoft::WRL;
namespace Ling {
	class D2D
	{
	public:
		~D2D();
		static D2D* get();
		Composition::CompositionDrawingSurface createDrawingSurface(const Composition::Compositor& comp);
		ComPtr<IDWriteTextLayout> createTextLayout(const std::wstring& text, const float& w, const float& h);
	private:
		D2D();
		void initFont();
		void initDevice();
		void initIcon();
	private:
		ComPtr<IDWriteFactory5> dwriteFactory;
		ComPtr<IDWriteTextFormat> baseTextFormat;
		ComPtr<IDWriteTextFormat> iconFormat;
		ComPtr<ID2D1Device> d2dDevice;
		ComPtr<ID2D1Factory1> d2dFactory;
		ComPtr<IDWriteFontCollection1> fontCollection;
	};
}

