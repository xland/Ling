#include <Windows.h>
#include "Ling/App.h"

using namespace Ling;

int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPTSTR lpCmdLine, _In_ int nCmdShow)
{
    App::init(hInstance);

    //LayoutDemo demo;
    //EventDemo demo;
    //InputDemo demo;
    //TextDemo demo;
    //demo.start();

    return App::exec();
}