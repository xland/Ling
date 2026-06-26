#pragma once
#include "Element.h"
namespace Ling{
	class ButtonIcon : public Element
	{
	public:
		ButtonIcon(WindowBase* win);
		~ButtonIcon();
	protected:
		void initProperty() override;
	private:
	};
}

