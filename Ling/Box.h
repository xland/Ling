#pragma once
#include "Element.h"
#include "Text.h"
namespace Ling{
	class WindowBase;
	class Box : public Element
	{
	public:
		Box(WindowBase* win);
		~Box();
	protected:
		void initProperty() override;
	private:
	private:
	};
}

