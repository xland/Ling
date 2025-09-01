#include <Windows.h>
#include <App.h>

#include "LayoutDemo.h"
#include "EventDemo.h"
#include "InputDemo.h"
#include "TextDemo.h"

int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPTSTR lpCmdLine, _In_ int nCmdShow)
{
    App::init(hInstance);
    
    //LayoutDemo demo;
    //EventDemo demo;
    //InputDemo demo;
    TextDemo demo;
    demo.start();

    return App::exec();
}