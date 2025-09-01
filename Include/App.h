#pragma once
#include <memory>
#include <Windows.h>
#include <yoga/Yoga.h>

namespace Ling
{
	class App
	{
        public:
            static void init(HINSTANCE hInstance);
            static App* get();
            static int exec();
        public:
            HINSTANCE hInstance;
        protected:
        private:
            App(HINSTANCE hInstance);
	};
}


