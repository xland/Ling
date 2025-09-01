#pragma once
#include <thread>
#include <Windows.h>

struct YGConfig;
class Element;
class SkFontMgr;
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
    static SkFontMgr* getFontMgr();
    static int exec();
public:
    friend class Element;
    HINSTANCE hInstance;
protected:
private:
    App(HINSTANCE hInstance);
    YGConfig* getLayoutConfig();
};

