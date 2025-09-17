#include <Windows.h>
#include <thorvg.h>
#include "../Include/WindowBase.h"

namespace Ling {
    int WindowBase::windowKeyDown(const int& key)
    {
        switch (key)
        {
        case VK_DELETE: {
            return 0;
        }
        case VK_LEFT: {
            return 0;
        }
        case VK_RIGHT: {
            return 0;
        }
        case VK_ESCAPE: {
            return 0;
        }
        case VK_SHIFT: {
            return 0;
        }
        case VK_CONTROL: {
            return 0;
        }
        }
        return 0;
    }
    int WindowBase::windowKeyUp(const int& key) {
        switch (key)
        {
        case VK_SHIFT: {
            return 0;
        }
        case VK_CONTROL: {
            return 0;
        }
        }
        return 0;
    }
}