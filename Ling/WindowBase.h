#pragma once
#include "WindowNative.h"
namespace Ling {
	class WindowBase:public WindowNative
	{
	public:
		WindowBase();
		virtual ~WindowBase();
		Text* makeText();
	public:
	protected:
	private:
	};
}