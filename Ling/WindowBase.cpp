#include "pch.h"
#include "WindowBase.h"
#include "WindowNative.h"
#include "Label.h"
namespace Ling{
    WindowBase::WindowBase():WindowNative()
    {

    }

    WindowBase::~WindowBase()
    {
    }
    Label* WindowBase::makeLabel()
    {
        auto ele = std::make_unique<Label>(this);
        auto result = ele.get();
        elements.push_back(std::move(ele));
        return result;
    }    
}