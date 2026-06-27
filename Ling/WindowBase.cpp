
#include "Header.h"
#include "WindowBase.h"
#include "WindowNative.h"
#include "Text.h"
#include "ButtonIcon.h"
#include "Box.h"
namespace Ling{
    WindowBase::WindowBase():WindowNative()
    {

    }

    WindowBase::~WindowBase()
    {
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