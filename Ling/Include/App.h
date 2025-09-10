#pragma once
#include <Windows.h>
#include <string>
#include <memory>

class Element;
namespace Ling{
    class App
    {
        public:
            App() = delete;
            App(const App&) = delete;                 // 删除拷贝构造函数
            App& operator=(const App&) = delete;      // 删除拷贝赋值函数
            App(App&&) = delete;                      // 删除移动构造函数
            App& operator=(App&&) = delete;           // 删除移动赋值函数
            ~App();
            static void init(HINSTANCE hInstance);
            static App* get();
            static int exec();
            static void loadSystemFont(const std::string & fontName);
            static void loadFontFile(const std::string& fileName);
        public:
            friend class Element;
            HINSTANCE hInstance;
        protected:
        private:
            App(HINSTANCE hInstance);
    };
}

