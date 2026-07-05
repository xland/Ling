#pragma once
#include "WindowNative.h"
namespace Ling {
	class Label;
	class WindowBase:public WindowNative
	{
	public:
		WindowBase();
		virtual ~WindowBase();
		Label* makeLabel();
	public:
	protected:
	private:
	};
}