#pragma once
#include "Node.h"
namespace Ling {
	class Slider : public Node
	{
	public:
		Slider(WinBase* win);
		~Slider();
	private:
		winrt::Windows::UI::Composition::CompositionDrawingSurface surface{ nullptr };
		void layout() override;
	};
}

