#pragma once
#include "Element.h"
#include "Text.h"
namespace Ling{
	class Box : public Element<Box>
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

