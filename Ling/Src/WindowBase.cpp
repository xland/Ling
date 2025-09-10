#include <yoga/Yoga.h>
#include <thorvg.h>
#include "../Include/Element.h"
#include "../Include/App.h"
#include "../Include/WindowBase.h"

namespace Ling {

    //static tvg::Text* text{ nullptr };

    WindowBase::WindowBase() : winPosition(0, 0),  winSize(980, 680), 
        canvas{ tvg::SwCanvas::gen() },  scene{ tvg::Scene::gen() }
    {
        Element::setWindow(this);
    }
    WindowBase::~WindowBase() {
        //scene->remove();
        //canvas->remove();
        //auto s = scene.release();
        //auto c = canvas.release();
        //delete(s);
        //delete(c);
    }
    void WindowBase::show() {
        ShowWindow(hwnd, SW_SHOW);     // 或者 SW_SHOWNORMAL / SW_SHOWDEFAULT
        UpdateWindow(hwnd);
        casecadeShown();
    }

    void WindowBase::insertChild(const int& index, Element* ele)
    {
        ElementBox::insertChild(index, ele);
        casecadeSetWindow(ele);
    }

    void WindowBase::addChild(Element* ele)
    {
        ElementBox::addChild(ele);
        casecadeSetWindow(ele);
    }

    void WindowBase::casecadeSetWindow(Element* ele)
    {
        ele->setWindow(this);
        auto box = dynamic_cast<ElementBox*>(ele);
        if (box) {
            for (auto e : *(box->getChildren()))
            {
                casecadeSetWindow(e);
            }
        }
    }
    void WindowBase::windowMouseMove(const int& x, const int& y)
    {
        MouseEvent e(x, y);
        auto ele = getElementByPosition(x, y);
        e.setRelativePosition(ele);
        if (hoverEle != ele) {
            if (hoverEle && !hoverEle->hittest(x, y)) {
                hoverEle->mouseLeave(e);
            }
            ele->mouseEnter(e);
            if (!hoverEle || hoverEle->getCursor() != ele->getCursor()) {
                SetCursor(LoadCursor(NULL, MAKEINTRESOURCE(ele->getCursor())));
            }
            hoverEle = ele;
        }
        ele->mouseMove(e);
    }

    void WindowBase::windowMouseDown(const int& x, const int& y, const MouseButton& mouseBtn)
    {
        MouseEvent e(x, y, mouseBtn);
        e.setRelativePosition(hoverEle);
        hoverEle->mouseDown(e);
    }

    void WindowBase::windowMouseUp(const int& x, const int& y, const MouseButton& mouseBtn)
    {
        MouseEvent e(x, y, mouseBtn);
        e.setRelativePosition(hoverEle);
        hoverEle->mouseUp(e);
    }

    const std::wstring& WindowBase::getWinClsName()
    {
        static std::wstring clsName = [] {
            WNDCLASSEXW wcex;
            wcex.cbSize = sizeof(WNDCLASSEX);
            wcex.style = CS_HREDRAW | CS_VREDRAW;
            wcex.lpfnWndProc = &WindowBase::routeWindowMsg;
            wcex.cbClsExtra = 0;
            wcex.cbWndExtra = 0;
            wcex.hInstance = App::get()->hInstance;
            wcex.hIcon = LoadIcon(wcex.hInstance, (LPCTSTR)IDI_WINLOGO);
            wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
            wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
            wcex.lpszMenuName = nullptr;
            wcex.lpszClassName = L"Ling";
            wcex.hIconSm = LoadIcon(wcex.hInstance, (LPCTSTR)IDI_WINLOGO);
            RegisterClassExW(&wcex);
            return wcex.lpszClassName;
            }();
        return clsName;
    }

    Element* WindowBase::getElementByPosition(int x, int y)
    {
        Element* result = this;
        auto children = result->getChildren();
        while (children && children->size() > 0) {
            bool flag{ false };
            for (auto child : *children) //遍历子元素
            {
                if (child->hittest(x, y)) //命中测试
                {
                    flag = true;
                    result = child;
                    children = result->getChildren();
                    break; //结束本层级的遍历，开始遍历下一个层级，找到最底层的元素
                }
            }
            if (!flag) {
                break;//上级命中，但本级没有命中，直接退出循环。
            }
        }
        return result;
    }
    void WindowBase::setFocusEle(Element* ele)
    {
        SetTimer(hwnd, FlashCaretTimer, 600, NULL); //每600毫秒触发一次
        SendMessage(hwnd, WM_TIMER, FlashCaretTimer, 0); //马上触发一次
        focusEle = ele;
    }
    size_t WindowBase::onDpiChanged(std::function<void()> callback)
    {
        dpiChangedCBId += 1;
        dpiChangedCBs.insert({ dpiChangedCBId,callback });
        return dpiChangedCBId;
    }
    void WindowBase::resetCanvas() {
        auto size = getWindowClientSize();
        setSize(size.w, size.h);//工作区大小
        int w = size.w * scaleFactor;
        int h = size.h * scaleFactor;
        buffer.resize(w * h);
        buffer.shrink_to_fit();
        canvas->target(buffer.data(), w, w, h, tvg::ColorSpace::ARGB8888);        
        if (canvas->paints().size() <= 0) {
            canvas->push(scene);
        }        
    }
    
    size_t WindowBase::onBeforeLayout(std::function<void()> callback) 
    {
        beforeLayoutCBId += 1;
        beforeLayoutCBs.insert({ beforeLayoutCBId,callback });
        return beforeLayoutCBId;
    }
    
    void WindowBase::layout()
    {
        for (const auto& pair : beforeLayoutCBs) {
            pair.second();
        }
        YGNodeCalculateLayout(node, YGUndefined, YGUndefined, YGDirectionLTR);
		ElementBox::layout();
    }
}
