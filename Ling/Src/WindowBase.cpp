#include <yoga/Yoga.h>
#include <thorvg.h>
#include "../Include/Util.h"
#include "../Include/Element.h"
#include "../Include/ElementBox.h"
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

    void WindowBase::insertChild(const int& index, const std::shared_ptr<Element>& ele)
    {
        ElementBox::insertChild(index, ele);
        casecadeSetWindow(ele.get());
    }

    void WindowBase::addChild(const std::shared_ptr<Element>& ele)
    {
        ElementBox::addChild(ele);
        casecadeSetWindow(ele.get());
    }

    void WindowBase::casecadeSetWindow(Element* ele)
    {
        ele->setWindow(this);
        auto box = dynamic_cast<ElementBox*>(ele);
        if (box) {
            for (auto e : *(box->getChildren()))
            {
                casecadeSetWindow(e.get());
            }
        }
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
        auto children = this->getChildren();
        while (children && children->size() > 0) 
        {
            bool findFlag = false;
            for (auto child : *children) //遍历子元素
            {
                if (child->hittest(x, y)) //命中测试
                {
                    result = child.get();
                    auto box = dynamic_cast<ElementBox*>(result);
                    if (!box) break; //已找到最底层元素了
                    children = box->getChildren(); //还要继续找
                    findFlag = true;
                }
            }
            if (!findFlag) break;
        }
        return result;
    }

    void WindowBase::setFocusEle(Element* ele)
    {
        SetTimer(hwnd, FlashCaretTimer, 600, NULL); //每600毫秒触发一次
        SendMessage(hwnd, WM_TIMER, FlashCaretTimer, 0); //马上触发一次
        focusEle = ele;
    }

    void WindowBase::resetCanvas() {
        auto size = getWindowClientSize();
        setSize(size.w, size.h);
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
    size_t WindowBase::onDpiChanged(std::function<void()> callback)
    {
        dpiChangedCBId += 1;
        dpiChangedCBs.insert({ dpiChangedCBId,callback });
        return dpiChangedCBId;
    }
    void WindowBase::dpiChanged(RECT* suggestedRect)
    {
        scene->scale(scaleFactor);
        for (const auto& pair : dpiChangedCBs) {
            pair.second();
        }
        auto w{ suggestedRect->right - suggestedRect->left };
        auto h{ suggestedRect->bottom - suggestedRect->top };
        SetWindowPos(hwnd, nullptr, suggestedRect->left, suggestedRect->top, w, h, SWP_NOZORDER | SWP_NOACTIVATE);
    }
    int WindowBase::paintArea()
    {
    //    App::loadSystemFont("SimHei");
    //    auto text = tvg::Text::gen();
    //    //https://github.com/thorvg/thorvg/wiki/25'-Development-Roadmap
    //    auto str = Ling::ConvertToUTF8(LR"(醉里挑灯看剑，梦回吹角连营。
    //八百里分麾下炙，五十弦翻塞外声。
    //沙场秋点兵。
    //马作的卢飞快，弓如霹雳弦惊。
    //了却君王天下事，赢得生前身后名。
    //可怜白发生！)");
    //    text->text(str.c_str());
    //    text->font("SimHei");
    //    text->size(21.f);
    //    text->fill(0, 0, 0);
    //    text->translate(22, 22);
    //    text->layout(600, 400);
    //    text->align(0.f, 0.f);
    //    canvas->push(text);


        canvas->update();
        canvas->draw();
        canvas->sync();
        auto size = getWindowClientSize();
        auto w = size.w * scaleFactor;
        auto h = size.h * scaleFactor;
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hwnd, &ps);
        BITMAPINFO bmi{};
        bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
        bmi.bmiHeader.biWidth = w;
        bmi.bmiHeader.biHeight = -h;  // top-down
        bmi.bmiHeader.biPlanes = 1;
        bmi.bmiHeader.biBitCount = 32;
        bmi.bmiHeader.biCompression = BI_RGB;
        SetStretchBltMode(hdc, HALFTONE);
        SetBrushOrgEx(hdc, 0, 0, nullptr);
        StretchDIBits(hdc, 0, 0, size.w, size.h, 0, 0, w, h,
            buffer.data(), &bmi, DIB_RGB_COLORS, SRCCOPY);
        EndPaint(hwnd, &ps);
        return 1;
    }
}
