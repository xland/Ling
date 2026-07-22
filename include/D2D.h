#pragma once
#include <d2d1_1.h>
#include <dwrite_3.h>
#include <d3d11.h>
#include <wrl.h>
#include <string>
#include <format>
#include <vector>
#include <winrt/Windows.UI.Composition.h>

namespace Ling {
	class D2D
	{
	public:
		~D2D();
		static D2D* get();
		static ID2D1Factory1* getFactory();
		winrt::Windows::UI::Composition::CompositionDrawingSurface createDrawingSurface(const winrt::Windows::UI::Composition::Compositor& comp, const float& w = 0, const float& h = 0);
		Microsoft::WRL::ComPtr<IDWriteTextLayout> createTextLayout(const std::wstring& text, const float& w, const float& h);
		void setEllipsis(IDWriteTextLayout* layout, const float& maxW, const float& maxH);
		Microsoft::WRL::ComPtr<ID2D1Bitmap> createBitmap(const std::wstring& imgPath);
		Microsoft::WRL::ComPtr<ID2D1PathGeometry> createPath(const std::vector<float>& points);
	private:
		D2D();
		void initFont();
		void initDevice();
		void initIcon();
	private:
		Microsoft::WRL::ComPtr<IDWriteFactory5> dwriteFactory;
		Microsoft::WRL::ComPtr<IDWriteTextFormat> baseTextFormat;
		Microsoft::WRL::ComPtr<IDWriteTextFormat> iconFormat;
		Microsoft::WRL::ComPtr<ID2D1Device> d2dDevice;
		Microsoft::WRL::ComPtr<ID2D1DeviceContext> deviceContext;
		Microsoft::WRL::ComPtr<ID2D1Factory1> d2dFactory;
		Microsoft::WRL::ComPtr<IDWriteFontCollection1> fontCollection;
	};
}