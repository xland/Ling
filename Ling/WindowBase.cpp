
#include "Header.h"
#include "WindowBase.h"
#include "Text.h"
#include "ButtonIcon.h"
#include "Box.h"
namespace Ling{
    WindowBase::WindowBase():compositor{ Composition::Compositor() }
    {

    }

    WindowBase::~WindowBase()
    {
    }

    

    BOOL WindowBase::onCursor()
    {
        if (hoverElement) {
            return hoverElement->setCursor();
        }
        return FALSE;
    }
    

    void WindowBase::setTitle(const std::wstring& title)
    {
        this->title = title;
    }

    std::wstring WindowBase::getTitle()
    {
        return title;
    }
    std::tuple<int, int> WindowBase::getPosition()
    {
        return std::make_tuple(x,y);
    }

    std::tuple<float, float> WindowBase::getSize()
    {
        return std::make_tuple(w, h);
    }
    void WindowBase::setSize(const float& w, const float& h)
    {
        this->w = w;
        this->h = h;
    }

    void WindowBase::setPosition(const int& xWin, const int& yWin)
    {
        this->x = x;
        this->y = y;
    }
    HWND WindowBase::getHandle()
    {
        return hwnd;
    }
    float WindowBase::getScaleFactor()
    {
        return dpi;
    }
    void WindowBase::setPosScreenCenter()
    {
        int screenWidth = GetSystemMetrics(SM_CXSCREEN);
        int screenHeight = GetSystemMetrics(SM_CYSCREEN);
        x = (screenWidth - w) / 2 ;
        y = (screenHeight - h) / 2;
    }
    Box* WindowBase::makeBox()
    {
        auto ele = std::make_unique<Box>(this);
        auto result = ele.get();
        elements.push_back(std::move(ele));
        return result;
    }
    Text* WindowBase::makeText()
    {
        auto ele = std::make_unique<Text>(this);
        auto result = ele.get();
        elements.push_back(std::move(ele));
        return result;
    }
    ButtonIcon* WindowBase::makeButtonIcon()
    {
        auto ele = std::make_unique<ButtonIcon>(this);
        auto result = ele.get();
        elements.push_back(std::move(ele));
        return result;
    }
}