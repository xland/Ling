#pragma once
#include <wrl.h>
#include <winrt/Windows.UI.Composition.h>
#include "Node.h"

namespace Ling {
	class WinBase;
	class NodeText :public Node
	{
		friend class WinBase;
	public:
		NodeText(Node* parent, const std::wstring& text);
		~NodeText();
		void onDown(void* e);
		void onUp(void* e);
		void onMove(void* e);
		void setAutoSize(float paddingLeftRight, float paddingTopBottom);
	public:
		winrt::Windows::UI::Composition::CompositionDrawingSurface surface{ nullptr };
		Microsoft::WRL::ComPtr<IDWriteTextLayout> textLayout;
	private:
	private:		
	};
}
