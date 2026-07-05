#include "pch.h"
#include "WindowBase.h"
#include "WindowNative.h"
#include "Text.h"
namespace Ling{
    WindowBase::WindowBase():WindowNative()
    {

    }

    WindowBase::~WindowBase()
    {
    }
    Text* WindowBase::makeText()
    {
        auto ele = std::make_unique<Text>(this);
        auto result = ele.get();
        elements.push_back(std::move(ele));
        return result;
    }
    
}