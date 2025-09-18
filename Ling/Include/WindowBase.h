#pragma once
#include <Windows.h>
#include <windowsx.h>
#include <string>
#include <memory>
#include "Size.h"
#include "ElementBox.h"

#define FlashCaretTimer WM_APP+1

namespace tvg {
	class SwCanvas;
	class Scene;
	class Text;
}
namespace Ling {
	class TextArea;
	class Label;
	class TextBlock;
	class WindowLayered;
	class WindowBase :public ElementBox
	{
	public:
		WindowBase();
		~WindowBase();
		virtual void show();
		virtual void createNativeWindow() = 0;
		void insertChild(const int& index, const std::shared_ptr<Element>& ele) override;
		void addChild(const std::shared_ptr<Element>& ele) override;
		void layout() override;
		size_t onDpiChanged(std::function<void()> callback);
		size_t onBeforeLayout(std::function<void()> callback);
	public:
		void setTitle(const std::wstring& title);
		const std::wstring& getTitle();
		const Position& getWindowPosition();
		const Size& getWindowSize();
		void setWindowSize(const int& w, const int& h);
		void setWindowPosition(const int& x, const int& y);
		const Size getWindowClientSize();
		void setWindowToScreenCenter();
		void resetWindowSize(const int& w, const int& h);
		void resetWindowPosition(const int& x, const int& y);
		void resetWindowToScreenCenter();
		Element* getElementByPosition(int x, int y);
		HWND getHandle();
		float getScaleFactor();
	public:
		friend class TextArea;
		friend class WindowBaseImpl;
		friend class Label;
		friend class Image;
		friend class TextBlock;
		friend class Element;
		friend class EffectBox;
		friend class WindowLayered;
	protected:
		const std::wstring& getWinClsName();
		void setScaleFactor();
		virtual LRESULT CALLBACK customMsgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
		void resetCanvas();
		virtual int paintArea();
	protected:
		HWND hwnd;
		std::wstring title;
		bool resizable{ true };
	private:
		static LRESULT CALLBACK routeWindowMsg(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
		LRESULT CALLBACK windowMsgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
		void windowMouseMove(const int& x, const int& y);
		void windowMouseDown(const int& x, const int& y, const MouseButton& mouseBtn);
		void windowMouseUp(const int& x, const int& y, const MouseButton& mouseBtn);
		int windowKeyDown(const int& key);
		int windowKeyUp(const int& key);
		void casecadeSetWindow(Element* ele);
		void setFocusEle(Element* ele);
		void dpiChanged(RECT* rect);
	private:
		/// <summary>
		/// 窗口在屏幕上的位置
		/// </summary>
		Position winPosition;
		/// <summary>
		/// 窗口的大小，
		/// 无边框窗口大小与Element大小一致
		/// 有边框窗口大小与Element大小不一致，因为有边框窗口大小包含边框和标题栏
		/// </summary>
		Size winSize;
		Element* hoverEle{ nullptr };
		Element* focusEle{ nullptr };
		float scaleFactor{ 1.f };
		tvg::SwCanvas* canvas;
		tvg::Scene* scene;
		std::vector<uint32_t> buffer;
		bool isWinInit{ true };

		size_t dpiChangedCBId{ 0 };
		size_t beforeLayoutCBId{ 0 };
		std::unordered_map<size_t, std::function<void()>> dpiChangedCBs;
		std::unordered_map<size_t, std::function<void()>> beforeLayoutCBs;
	};
}

