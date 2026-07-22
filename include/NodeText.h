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
	public:
	private:
		NodeText(WinBase* win);
	private:		
	};
}
