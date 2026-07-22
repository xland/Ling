#pragma once

#include "Node.h"

namespace Ling {
	class WinBase;
	class NodeText :public Node
	{
		friend class WinBase;
	public:
		~NodeText();
		void onDown(void* e);
		void onUp(void* e);
		void onMove(void* e);
		void setAutoSize(float paddingLeftRight, float paddingTopBottom);
	public:
		Composition::CompositionDrawingSurface surface{ nullptr };
		ComPtr<IDWriteTextLayout> textLayout;
	private:
		NodeText(WinBase* win,const std::wstring& text);
	private:		
	};
}
