#pragma once
#include <d2d1_1.h>
#include <dwrite_3.h>
#include <d3d11.h>
#include <dxgi1_2.h>
#include <wrl.h>
#include <string>
#include <format>
#include <vector>
#include <winrt/Windows.UI.Composition.h>

namespace Ling {
	class D2D
	{
		friend class App;
	public:
		~D2D();
		static D2D* get();
		void addFonts(const std::vector<std::wstring>& fontResourceNames);
		winrt::Windows::UI::Composition::CompositionDrawingSurface createDrawingSurface(const winrt::Windows::UI::Composition::Compositor& comp, float w = 0, float h = 0);

		// 创建一条用于合成的 swap chain（双缓冲 + FLIP_DISCARD）。
		// CompositionDrawingSurface 是单缓冲的：BeginDraw 拿到的就是合成器正在读的那张纹理，
		// 一帧里"擦干净→逐个重画"的中间态可能被采样到，高频重绘时表现为整帧闪烁。
		// swap chain 则是画在后台缓冲上、Present 时才整帧原子换出，适合每帧重画的窗口。
		// 代价是多一块与窗口等大的纹理，所以只给需要的窗口用，图标/文本仍走 createDrawingSurface。
		Microsoft::WRL::ComPtr<IDXGISwapChain1> createSwapChain(UINT w, UINT h);
		// 把 swap chain 包成能塞给 CreateSurfaceBrush 的合成表面，视觉树用法与 DrawingSurface 完全一致。
		winrt::Windows::UI::Composition::ICompositionSurface createSurfaceForSwapChain(const winrt::Windows::UI::Composition::Compositor& comp, IDXGISwapChain1* swap);
	public:
		Microsoft::WRL::ComPtr<ID3D11Device> d3dDevice;
		Microsoft::WRL::ComPtr<IDXGIFactory2> dxgiFactory;
		Microsoft::WRL::ComPtr<ID2D1Device> d2dDevice;
		Microsoft::WRL::ComPtr<ID2D1DeviceContext> deviceContext;
		Microsoft::WRL::ComPtr<ID2D1Factory1> d2dFactory;
		Microsoft::WRL::ComPtr<IDWriteFactory5> dwriteFactory;
		Microsoft::WRL::ComPtr<IDWriteTextFormat> baseTextFormat;
		Microsoft::WRL::ComPtr<IDWriteFontCollection1> fontCollection;
	private:
		D2D();
		static void init();
		void initFont();
		void initDevice();

	};
}