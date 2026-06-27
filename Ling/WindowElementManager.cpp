#include "pch.h"
#include "WindowElementManager.h"
#include "Box.h"
#include "WindowBase.h"
namespace Ling {
    WindowElementManager::WindowElementManager():compositor{ Composition::Compositor() }
    {
    }
    WindowElementManager::~WindowElementManager()
    {
    }
    
    void WindowElementManager::layout(const float& w, const float& h)
    {
        YGNodeStyleSetWidth(body->node, w);
        YGNodeStyleSetHeight(body->node, h);
        YGNodeCalculateLayout(body->node, w, h, YGDirectionLTR);
        body->layout();
    }
    void WindowElementManager::initBody()
    {
        body = std::make_unique<Box>((WindowBase*)this);
        Color c(0xFFFFFFFF);
        body->visual.Brush(compositor.CreateColorBrush(c.getUIColor()));
        body->setCursor(IDC_ARROW);
    }
    void WindowElementManager::mouseEnterElement(const int& x, const int& y)
    {
        IElement* newHover = body->hitTest(x, y);
        if (newHover != hoverElement) {
            MouseEvent event(x, y);
            if (hoverElement) {
                // 查找老的hoverElement和新的newHover的共同祖先
                IElement* commonAncestor = hoverElement->findAncestor(newHover);
                // 从老的hoverElement向上触发mouseLeave，直到共同祖先
                IElement* current = hoverElement;
                while (current && current != commonAncestor) {
                    current->mouseLeave(event);
                    current = current->parent;
                }
            }
            if (newHover) {
                // 查找共同祖先
                IElement* commonAncestor = hoverElement ? newHover->findAncestor(hoverElement) : nullptr;
                // 从新 hover 元素向上触发 mouseEnter，直到共同祖先（不包含）
                std::vector<IElement*> pathToEnter;
                IElement* current = newHover;
                while (current && current != commonAncestor) {
                    pathToEnter.push_back(current);
                    current = current->parent;
                }
                // 从外到内触发（父元素先于子元素触发 enter）
                for (auto it = pathToEnter.rbegin(); it != pathToEnter.rend(); ++it) {
                    (*it)->mouseEnter(event);
                }
            }
            hoverElement = newHover;
        }
        if (newHover) {
            newHover->mouseMove(MouseEvent(x, y));
        }
    }
    void WindowElementManager::mouseLeaveElement()
    {
        if (hoverElement) {
            MouseEvent event(0, 0);  // 离开时坐标不重要
            IElement* current = hoverElement;
            while (current) {
                current->mouseLeave(event);
                current = current->parent;
            }
            hoverElement = nullptr;
        }
    }
    BOOL WindowElementManager::setCursor()
    {
        if (hoverElement) {
            return hoverElement->setCursor();
        }
        return FALSE;
    }
}