#pragma once
#include <d2d1_1.h>
#include <dwrite_3.h>
#include <d3d11.h>
#include <dxgi1_2.h>
#include <wrl.h>
#include <string>
#include <format>
#include <vector>
#include <unordered_map>
#include <winrt/Windows.UI.Composition.h>

namespace Ling {
	class D2D
	{
	public:
		~D2D();
		// 取图形设备。设备是惰性建的：第一次调用时才建，所以启动到托盘、一个窗口都没有的
		// 状态下进程里根本没有 D3D / D2D / DXGI / DWrite 这一套东西。
		// 最后一个窗口关掉时设备会被销毁（见 WinBase 的析构），下次调用这里自动重建
		static D2D* get();
		// 销毁整套设备，把显卡驱动在本进程里的分配一并还给系统。调用方持有的位图 / 画刷 /
		// 字体会随之失效，所以只能在确认没人再用它们的时候调 —— 也就是没有任何窗口的时候
		static void dispose();
		// 追加自定义字体（图标字体这类），资源名即 vcxproj 里的资源标识。
		// 只登记资源名、不触碰设备，因此启动时调它不会把整套 D3D 拉起来；设备每次重建
		// 都会按登记的名字重新加载一遍。
		// 它们单独成一个集合，不与系统字体合并 —— 合并要把系统字体集在进程内重建一份，
		// 元数据能占好几 MB。代价是取 TextFormat 时必须按族名分流，见 getTextFormat
		static void addFonts(const std::vector<std::wstring>& fontResourceNames);
		// 按字体族名挑 TextFormat：族名属于 addFonts 加进来的自定义字体，就给绑了自定义
		// 集合的那个；其余（空族名、系统字体名）一律给 baseTextFormat。
		// 之所以要分流：TextFormat 创建时就把字体集合定死了，而 IDWriteTextLayout::
		// SetFontFamilyName 只能在同一集合内换族名、换不了集合 —— 拿系统那个 format
		// 建出来的 layout 再设图标族名是找不到的，会退回默认字体（图标显示成方块）。
		// 返回值永不为空（最差也是 baseTextFormat），寿命跟着 D2D 单例，调用方不必持有
		IDWriteTextFormat* getTextFormat(const std::wstring& fontFamily);
		winrt::Windows::UI::Composition::CompositionDrawingSurface createDrawingSurface(const winrt::Windows::UI::Composition::Compositor& comp, float w = 0, float h = 0);

		// 创建一条用于合成的 swap chain（双缓冲 + FLIP_DISCARD）。
		// CompositionDrawingSurface 是单缓冲的：BeginDraw 拿到的就是合成器正在读的那张纹理，
		// 一帧里"擦干净→逐个重画"的中间态可能被采样到，高频重绘时表现为整帧闪烁。
		// swap chain 则是画在后台缓冲上、Present 时才整帧原子换出，适合每帧重画的窗口。
		// 代价是多一块与窗口等大的纹理，所以只给需要的窗口用，图标/文本仍走 createDrawingSurface。
		Microsoft::WRL::ComPtr<IDXGISwapChain1> createSwapChain(UINT w, UINT h);
		// 把 swap chain 包成能塞给 CreateSurfaceBrush 的合成表面，视觉树用法与 DrawingSurface 完全一致。
		winrt::Windows::UI::Composition::ICompositionSurface createSurfaceForSwapChain(const winrt::Windows::UI::Composition::Compositor& comp, IDXGISwapChain1* swap);
		// 直接建一个 IDWriteTextLayout。Ling 的 Label/Text 走的是 Composition 节点树，
		// 而 CutMask / WinLong 是在自己的 D2D context 上手绘文字，需要裸的 layout。
		static Microsoft::WRL::ComPtr<IDWriteTextLayout> makeTextLayout(const std::wstring& text, float fontSize, float w= FLT_MAX, float h= FLT_MAX);
	public:
		Microsoft::WRL::ComPtr<ID3D11Device> d3dDevice;
		Microsoft::WRL::ComPtr<IDXGIFactory2> dxgiFactory;
		Microsoft::WRL::ComPtr<ID2D1Device> d2dDevice;
		Microsoft::WRL::ComPtr<ID2D1DeviceContext> deviceContext;
		Microsoft::WRL::ComPtr<ID2D1Factory1> d2dFactory;
		// 自定义字体的族名（小写）-> 绑了 customFontCollection 的 TextFormat。
		// 族名不是猜的，是从字体文件里枚举出来的，各本地化名字都登记了一份
		std::unordered_map<std::wstring, Microsoft::WRL::ComPtr<IDWriteTextFormat>> customFormats;
		// 自定义字体的内存加载器。必须活到集合和 layout 都不用了为止，所以存成员而不是局部变量
		Microsoft::WRL::ComPtr<IDWriteInMemoryFontFileLoader> fontLoader;
		Microsoft::WRL::ComPtr<IDWriteFactory5> dwriteFactory;
		// 默认 TextFormat，用的是 DWrite 的系统字体集合（CreateTextFormat 时传 nullptr）
		Microsoft::WRL::ComPtr<IDWriteTextFormat> baseTextFormat;
		// 只装 addFonts 加进来的自定义字体，不含系统字体
		Microsoft::WRL::ComPtr<IDWriteFontCollection1> customFontCollection;
	private:
		D2D();
		void initFont();
		void initDevice();
		// 按 fontNames 里登记的资源名重建自定义字体集合。设备每次重建都要跑一遍，
		// 所以是全量重建而不是增量追加
		void loadFonts();
		// 按 customFontCollection 里实际有的字体族名，逐族建一个 TextFormat 登记进 customFormats
		void makeCustomFormats();
		// addFonts 登记的字体资源名。存成静态的，这样设备销毁重建之后自定义字体不会丢
		static std::vector<std::wstring> fontNames;

	};
}