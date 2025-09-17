#include <yoga/Yoga.h>
#include <thorvg.h>
#include "../Include/Element.h"
#include "../Include/ElementBox.h"
#include "../Include/App.h"
#include "../Include/WindowBase.h"

namespace Ling {
    void WindowBase::windowMouseMove(const int& x, const int& y)
    {
        MouseEvent e(x, y);
        //先快速检查当前 hover 元素是否仍命中，避免鼠标移动进入 getElementByPosition
        if (hoverEle && !hoverEle->hittest(x, y)) {  
            e.setRelativePosition(hoverEle);
            hoverEle->mouseMove(e);
            auto ele = hoverEle->getParent();
            while (ele) {
                ele->mouseMove(e);
                ele = ele->getParent(); //事件冒泡
            }
            return;
        }        
        auto ele = getElementByPosition(x, y);
        e.setRelativePosition(ele);
        if(hoverEle) hoverEle->mouseLeave(e);
        ele->mouseEnter(e);
        if (!hoverEle || hoverEle->getCursor() != ele->getCursor()) {
            SetCursor(LoadCursor(NULL, MAKEINTRESOURCE(ele->getCursor())));
        }
        hoverEle = ele;
        ele->mouseMove(e);
        ele = hoverEle->getParent();
        while (ele) {
            ele->mouseMove(e);
            ele = ele->getParent(); //事件冒泡
        }
    }

    void WindowBase::windowMouseDown(const int& x, const int& y, const MouseButton& mouseBtn)
    {
        MouseEvent e(x, y, mouseBtn);
        e.setRelativePosition(hoverEle);
        hoverEle->mouseDown(e);

        auto ele = hoverEle->getParent();
        while (ele) {
            ele->mouseDown(e);
            ele = ele->getParent();
        }
    }

    void WindowBase::windowMouseUp(const int& x, const int& y, const MouseButton& mouseBtn)
    {
        MouseEvent e(x, y, mouseBtn);
        e.setRelativePosition(hoverEle);
        hoverEle->mouseUp(e);
        auto ele = hoverEle->getParent();
        while (ele) {
            ele->mouseUp(e);
            ele = ele->getParent();
        }
    }
}