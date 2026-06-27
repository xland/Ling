#pragma once

#include "pch.h"
#include "WindowNative.h"
namespace Ling {
	class WindowBase:public WindowNative
	{
	public:
		WindowBase();
		virtual ~WindowBase();
		Box* makeBox();
		Text* makeText();
		ButtonIcon* makeButtonIcon();
	public:
	protected:
	private:
	};
}