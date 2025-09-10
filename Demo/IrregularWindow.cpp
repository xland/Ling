#include <../Ling/Include/WindowLayered.h>

#include "IrregularWindow.h"


IrregularWindow::IrregularWindow()
{
    win = std::make_unique<WindowLayered>();
    //win = std::make_unique<WindowNormal>();
    win->setTitle(L"SkiaGUI 示例窗口");
    win->setWindowSize(800, 800);
    win->setWindowToScreenCenter();
    win->setBackgroundColor(0x00000000);
    win->createNativeWindow();
}
IrregularWindow::~IrregularWindow()
{

}
void IrregularWindow::start()
{
    auto center = new Element();
    center->setSize(800, 800);
    center->setBackgroundColor(0x876543FF);
    center->setRadius(400);
    center->setCaptionFlag(true);
    win->addChild(center);

    win->layout();
    win->show();
}