#include <../Ling/Include/WindowLayered.h>

#include "IrregularWindow.h"


IrregularWindow::IrregularWindow()
{
    win = std::make_unique<WindowLayered>();
    //win = std::make_unique<WindowNormal>();
    win->setTitle(L"SkiaGUI 示例窗口");
    win->setWindowSize(800, 800);
    win->setWindowToScreenCenter();
    win->setBackgroundColor(0x222222FF);
    win->createNativeWindow();
}
IrregularWindow::~IrregularWindow()
{

}
void IrregularWindow::start()
{
    win->setRadius(400);
    win->layout();
    win->show();
}