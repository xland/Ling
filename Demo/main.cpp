#include <Windows.h>
#include "../Ling/Include/App.h"

#include "LayoutDemo.h"
#include "EventDemo.h"
#include "InputDemo.h"
#include "TextDemo.h"
#include "IrregularWindow.h"
#include "GraphicsDemo.h"

int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPTSTR lpCmdLine, _In_ int nCmdShow)
{
    App::init(hInstance);
    //LayoutDemo demo;
    //EventDemo demo;
    //InputDemo demo;
    //TextDemo demo;
    //IrregularWindow demo;
    GraphicsDemo demo;
    demo.start();

    return App::exec();
}