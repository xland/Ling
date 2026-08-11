#include "pch.h"
#include "../include/TextBox.h"
#include "../include/WinBase.h"
#include "../include/Canvas.h"
#include "../include/D2D.h"
#include "../include/Util.h"

namespace Ling {

    // 逻辑像素常量（内部乘 dpi）。
    constexpr float caretW{ 1.f };          // 光标线宽
    constexpr float scrollBarW{ 6.f };      // 滚动条宽度
    constexpr float scrollThumbMinH{ 20.f };// 滚动条滑块最小高度
    // 光标闪烁周期的半程（毫秒）：每次定时器到点翻转一次可见性。
    constexpr UINT caretBlinkMs{ 500 };
    // Tab 键插入的空格数。DWrite 的制表位与自动换行配合不好，直接换成空格更可控。
    constexpr int tabSpaces{ 4 };
    // 定时器 id 分配起点。WinBase::setTimer 内部会加 WM_APP，这里只需保证同窗口内不重复。
    constexpr UINT timerIdBase{ 0x4200 };

    TextBox::TextBox(WinBase* win) : Node(win)
    {
        // 每个实例一个独立的定时器 id。见头文件里的说明。
        static UINT idSeed{ timerIdBase };
        timerId = idSeed++;

        setWidth(240.f);
        setHeight(120.f);
        setPadding(6.f);

        canvas = Node::makeChild<Canvas>();
        // 绝对定位 + 四边 inset 全 0：Yoga 会把普通子节点摆进内边距以内，
        // 而滚动条要紧贴右边框，所以 canvas 得盖住整个边框盒。
        // 绝对定位子节点的 inset 只叠父节点的 border、不叠 padding（见 AbsoluteLayout.cpp），
        // 于是 inset 为 0 就正好是边框盒；宽高由 left/right、top/bottom 撑开，不要再设尺寸。
        canvas->setPositionType(Position::Absolute);
        canvas->setPosition(Edge::Left, 0.f);
        canvas->setPosition(Edge::Top, 0.f);
        canvas->setPosition(Edge::Right, 0.f);
        canvas->setPosition(Edge::Bottom, 0.f);
        // 必须在首次 startPaint() 之前调用。
        canvas->enableSwapChain();

        buildLayout();

        auto weakThis = getWeakThis();
        downTok   = win->onMouseDown .add([this, weakThis](POINT pos, bool isRight) { if (!weakThis.lock()) return; onDown(pos, isRight); });
        moveTok   = win->onMouseMove .add([this, weakThis](POINT pos)               { if (!weakThis.lock()) return; onMove(pos); });
        upTok     = win->onMouseUp   .add([this, weakThis](POINT pos, bool isRight) { if (!weakThis.lock()) return; onUp(pos, isRight); });
        wheelTok  = win->onMouseWheel.add([this, weakThis](POINT pos, float space)  { if (!weakThis.lock()) return; onWheel(pos, space); });
        keyTok    = win->onKeyDown   .add([this, weakThis](UINT key)                { if (!weakThis.lock()) return; onKey(key); });
        charTok   = win->onChar      .add([this, weakThis](UINT code)               { if (!weakThis.lock()) return; onCharInput(code); });
        imeTok    = win->onIME       .add([this, weakThis]()                        { if (!weakThis.lock()) return; onImeStart(); });
        cursorTok = win->onCursor    .add([this, weakThis](bool* handled)           { if (!weakThis.lock()) return; onCursorEvent(handled); });
        tickTok   = win->onTimer     .add([this, weakThis](UINT id)                 { if (!weakThis.lock()) return; onTick(id); });
        // 宿主窗口失焦时控件一并失焦，否则光标会在没焦点的窗口里继续闪。
        blurTok   = win->onBlur      .add([this, weakThis]()                        { if (!weakThis.lock()) return; blur(); });
    }

    TextBox::~TextBox()
    {
        if (focused) win->killTimer(timerId);
        win->onMouseDown .remove(downTok);
        win->onMouseMove .remove(moveTok);
        win->onMouseUp   .remove(upTok);
        win->onMouseWheel.remove(wheelTok);
        win->onKeyDown   .remove(keyTok);
        win->onChar      .remove(charTok);
        win->onIME       .remove(imeTok);
        win->onCursor    .remove(cursorTok);
        win->onTimer     .remove(tickTok);
        win->onBlur      .remove(blurTok);
    }

    // ---- 属性 -----------------------------------------------------------------

    void TextBox::setText(const std::wstring& val)
    {
        text = val;
        buildLayout();
        // 光标夹到新文本范围内，选区一并清掉。
        caretIndex = std::clamp(caretIndex, 0, (int)text.length());
        selAnchor = caretIndex;
        resetCaretPos(caretIndex);
        setScrollY(scrollY);
        win->refresh();
        onTextChanged(this, text);
    }

    std::wstring TextBox::getText()
    {
        return text;
    }

    void TextBox::setFontSize(float val)
    {
        fontSize = val;
        buildLayout();
        resetCaretPos(caretIndex);
        win->refresh();
    }

    void TextBox::setFontFamily(const std::wstring& val)
    {
        fontFamily = val;
        buildLayout();
        resetCaretPos(caretIndex);
        win->refresh();
    }

    void TextBox::setColor(Color val)
    {
        color = val;
        // 没单独指定光标色时，光标跟着文字色走。
        if (!hasCaretColor) caretColor = val;
        win->refresh();
    }

    void TextBox::setCaretColor(Color val)
    {
        caretColor = val;
        hasCaretColor = true;
        win->refresh();
    }

    void TextBox::setSelectionBgColor(Color val)
    {
        selectionBgColor = val;
        win->refresh();
    }

    void TextBox::setPlaceholder(const std::wstring& val)
    {
        placeholder = val;
        buildLayout();
        win->refresh();
    }

    void TextBox::setPlaceholderColor(Color val)
    {
        placeholderColor = val;
        win->refresh();
    }

    void TextBox::selectAll()
    {
        selAnchor = 0;
        caretIndex = (int)text.length();
        resetCaretPos(caretIndex);
        scrollCaretIntoView();
        win->refresh();
    }

    bool TextBox::isFocused() const
    {
        return focused;
    }

    // ---- 文本布局 -------------------------------------------------------------

    float TextBox::contentW() const
    {
        const float d = win->dpi;
        // getPadding 返回的是逻辑像素，w 是物理像素。
        auto [pl, pt, pr, pb] = const_cast<TextBox*>(this)->getPadding();
        return std::max(0.f, w - (pl + pr) * d);
    }

    float TextBox::contentH() const
    {
        const float d = win->dpi;
        auto [pl, pt, pr, pb] = const_cast<TextBox*>(this)->getPadding();
        return std::max(0.f, h - (pt + pb) * d);
    }

    float TextBox::textH() const
    {
        if (!textLayout) return 0.f;
        DWRITE_TEXT_METRICS m{};
        textLayout->GetMetrics(&m);
        return m.height;
    }

    void TextBox::createLayouts(float maxW)
    {
        auto d2d = D2D::get();
        if (!d2d) return;
        const float px = fontSize * win->dpi;

        d2d->dwriteFactory->CreateTextLayout(text.data(), (UINT32)text.length(),
            d2d->baseTextFormat.Get(), maxW, FLT_MAX, textLayout.ReleaseAndGetAddressOf());
        if (textLayout) {
            textLayout->SetFontSize(px, { 0, INT_MAX });
            if (!fontFamily.empty()) textLayout->SetFontFamilyName(fontFamily.data(), { 0, INT_MAX });
        }

        if (!placeholder.empty()) {
            d2d->dwriteFactory->CreateTextLayout(placeholder.data(), (UINT32)placeholder.length(),
                d2d->baseTextFormat.Get(), maxW, FLT_MAX, placeholderLayout.ReleaseAndGetAddressOf());
            if (placeholderLayout) {
                placeholderLayout->SetFontSize(px, { 0, INT_MAX });
                if (!fontFamily.empty()) placeholderLayout->SetFontFamilyName(fontFamily.data(), { 0, INT_MAX });
            }
        }
        else {
            placeholderLayout.Reset();
        }
    }

    void TextBox::buildLayout()
    {
        // 宽度已知时按内容区宽度折行；布局还没跑过（w 为 0）时先用 FLT_MAX，
        // 等 layout() 拿到真实宽度再重建。
        const float cw = contentW();
        createLayouts(cw > 0.f ? cw : FLT_MAX);
        // 第二趟：滚动条贴在右边框上，先占掉右内边距 —— 只有它比右内边距还宽时，
        // 才需要再挤占文本宽度，否则右侧文字会被压住。
        // 收窄只会让文本更高，所以不会和 needScrollBar 来回抖动。
        if (cw > 0.f && needScrollBar()) {
            const float overlap = scrollBarW * win->dpi - getPaddingRight() * win->dpi;
            if (overlap > 0.f) createLayouts(std::max(1.f, cw - overlap));
        }
        lastLayoutW = cw;
    }

    void TextBox::layout()
    {
        Node::layout();
        // 控件宽度变了（窗口 resize / flex 重排）要按新宽度重新折行。
        if (contentW() != lastLayoutW) {
            buildLayout();
            resetCaretPos(caretIndex);
        }
        // 高度可能变小，滚动量要重新夹紧。
        setScrollY(scrollY);
        paint();
    }

    void TextBox::onDpiChanged()
    {
        // 字号、内边距都是 dpi 派生量，重建 layout 即可；
        // applyDpiChange 之后 WinBase 会 relayout，paint 在 layout() 里发生。
        buildLayout();
        resetCaretPos(caretIndex);
    }

    // ---- 光标与选区 -----------------------------------------------------------

    void TextBox::resetCaretPos(int textIndex)
    {
        if (!textLayout) return;
        FLOAT cx{ 0.f }, cy{ 0.f };
        DWRITE_HIT_TEST_METRICS hm{};
        // 从文本位置推算坐标。isTrailingHit 传 FALSE，取字符的前导边。
        textLayout->HitTestTextPosition(textIndex, FALSE, &cx, &cy, &hm);
        caretPos.x = cx;
        caretPos.y = cy;
        caretHeight = hm.height;
        caretVisible = true;
    }

    int TextBox::hitTextIndex(POINT pos) const
    {
        if (!textLayout) return 0;
        const float d = win->dpi;
        auto [pl, pt, pr, pb] = const_cast<TextBox*>(this)->getPadding();
        // 窗口坐标 -> 内容坐标：减掉控件原点与内边距，再加回滚动量。
        const float lx = pos.x - (x + pl * d);
        const float ly = pos.y - (y + pt * d) + scrollY;
        BOOL isTrailingHit{ FALSE }, isInside{ FALSE };
        DWRITE_HIT_TEST_METRICS hm{};
        textLayout->HitTestPoint(lx, ly, &isTrailingHit, &isInside, &hm);
        int index = (int)hm.textPosition;
        // 点在字符后半边时落到它的后一个位置。
        if (isTrailingHit) index += (int)hm.length;
        return std::clamp(index, 0, (int)text.length());
    }

    void TextBox::moveCaret(int index, bool extendSel)
    {
        caretIndex = std::clamp(index, 0, (int)text.length());
        if (!extendSel) selAnchor = caretIndex;
        resetCaretPos(caretIndex);
        scrollCaretIntoView();
        win->refresh();
    }

    int TextBox::selStart() const { return std::min(selAnchor, caretIndex); }
    int TextBox::selEnd()   const { return std::max(selAnchor, caretIndex); }
    bool TextBox::hasSelection() const { return selAnchor != caretIndex; }

    bool TextBox::delSelection()
    {
        if (!hasSelection()) return false;
        const int start = selStart();
        text.erase(start, selEnd() - start);
        caretIndex = start;
        selAnchor = start;
        buildLayout();
        resetCaretPos(caretIndex);
        scrollCaretIntoView();
        win->refresh();
        onTextChanged(this, text);
        return true;
    }

    void TextBox::insertText(const std::wstring& val)
    {
        if (val.empty()) return;
        // 有选区则先替换掉。delSelection 内部已经重建过 layout，这里插入后会再建一次，
        // 多一次重建换取逻辑清晰 —— 输入是低频操作，不值得为此把两件事揉在一起。
        delSelection();
        text.insert(caretIndex, val);
        caretIndex += (int)val.length();
        selAnchor = caretIndex;
        buildLayout();
        resetCaretPos(caretIndex);
        scrollCaretIntoView();
        win->refresh();
        onTextChanged(this, text);
    }

    // ---- 滚动 -----------------------------------------------------------------

    float TextBox::maxScrollY() const
    {
        return std::max(0.f, textH() - contentH());
    }

    bool TextBox::needScrollBar() const
    {
        return contentH() > 0.f && textH() > contentH();
    }

    bool TextBox::isPosInScrollBar(POINT pos) const
    {
        if (!needScrollBar()) return false;
        const float d = win->dpi;
        // 滑轨紧贴控件右边框、纵向占满整个控件高度，四边都不留内边距。
        return pos.x >= x + w - scrollBarW * d && pos.x < x + w && pos.y >= y && pos.y < y + h;
    }

    bool TextBox::isPosInThumb(POINT pos) const
    {
        if (!isPosInScrollBar(pos)) return false;
        const float top = thumbY();
        return pos.y >= y + top && pos.y < y + top + thumbH();
    }

    float TextBox::thumbH() const
    {
        // 滑块长度 = 滑轨长度 × 可视比例（可视高 / 内容总高）。
        return std::round(std::max(scrollThumbMinH * win->dpi, h * contentH() / textH()));
    }

    float TextBox::thumbY() const
    {
        const float maxScroll = maxScrollY();
        if (maxScroll <= 0.f) return 0.f;
        // 滑轨占满整个控件高度，所以没滚动时贴顶边、滚到底时贴底边。
        return std::round((h - thumbH()) * scrollY / maxScroll);
    }

    void TextBox::setScrollY(float val)
    {
        // snap 到整像素：带小数的偏移会让 ClearType 文本在滚动时发糊（同 ScrollerBox）。
        const float next = std::round(std::clamp(val, 0.f, maxScrollY()));
        if (next == scrollY) return;
        scrollY = next;
        win->refresh();
    }

    void TextBox::scrollCaretIntoView()
    {
        const float visH = contentH();
        if (visH <= 0.f) return;
        if (caretPos.y < scrollY) {
            setScrollY(caretPos.y);
        }
        else if (caretPos.y + caretHeight > scrollY + visH) {
            setScrollY(caretPos.y + caretHeight - visH);
        }
    }

    // ---- 焦点 -----------------------------------------------------------------

    void TextBox::focus()
    {
        if (focused) return;
        focused = true;
        caretVisible = true;
        // 键盘消息发给宿主窗口，先确保它自己有焦点。
        SetFocus(win->hwnd);
        win->setTimer(caretBlinkMs, timerId);
        win->refresh();
    }

    void TextBox::blur()
    {
        if (!focused) return;
        focused = false;
        caretVisible = false;
        // 失焦时清掉选区：留着高亮会让人以为还能编辑。
        selAnchor = caretIndex;
        win->killTimer(timerId);
        win->refresh();
    }

    // ---- 绘制 -----------------------------------------------------------------

    void TextBox::paint()
    {
        if (!canvas) return;
        auto ctx = canvas->startPaint();
        if (!ctx) return;
        // swap chain 的后台缓冲里是上一轮的旧内容，必须自己清屏。
        // 背景由 Node 的 setBg 画在父 visual 上，这里保持透明。
        ctx->Clear(0);

        const float d = win->dpi;
        auto [pl, pt, pr, pb] = getPadding();
        // canvas 是绝对定位、靠负 inset 撑满整个边框盒的，所以它的原点就是控件原点，
        // 尺寸就是 w × h —— 文本要自己偏移 padding，滚动条则能一直贴到右边框。
        const float ox = pl * d;
        const float oy = pt * d;
        const float cw = contentW();
        const float ch = contentH();

        ComPtr<ID2D1SolidColorBrush> brush;
        ctx->CreateSolidColorBrush(color.getD2DColor(), brush.GetAddressOf());

        // 裁剪到内容区，滚动时溢出的文本不会画到内边距上。
        ctx->PushAxisAlignedClip(D2D1::RectF(ox, oy, ox + cw, oy + ch), D2D1_ANTIALIAS_MODE_ALIASED);

        if (text.empty() && placeholderLayout && !focused) {
            ComPtr<ID2D1SolidColorBrush> phBrush;
            ctx->CreateSolidColorBrush(placeholderColor.getD2DColor(), phBrush.GetAddressOf());
            ctx->DrawTextLayout({ ox, oy }, placeholderLayout.Get(), phBrush.Get(), D2D1_DRAW_TEXT_OPTIONS_NONE);
        }
        else if (textLayout) {
            if (focused && hasSelection()) {
                ComPtr<ID2D1SolidColorBrush> selBrush;
                ctx->CreateSolidColorBrush(selectionBgColor.getD2DColor(), selBrush.GetAddressOf());
                // 选区底色画在文字底下，所以先画。坐标系整体平移到文本原点，
                // 这样 HitTestTextRange 返回的布局坐标可以直接用。
                ctx->SetTransform(D2D1::Matrix3x2F::Translation(ox, oy - scrollY));
                paintSelectionBg(ctx, selBrush.Get());
                ctx->SetTransform(D2D1::Matrix3x2F::Identity());
            }
            ctx->DrawTextLayout({ ox, oy - scrollY }, textLayout.Get(), brush.Get(), D2D1_DRAW_TEXT_OPTIONS_NONE);
            if (focused && caretVisible) {
                ComPtr<ID2D1SolidColorBrush> caretBrush;
                ctx->CreateSolidColorBrush(caretColor.getD2DColor(), caretBrush.GetAddressOf());
                // DrawLine 是以线宽中心对齐的，光标在行首时左半边会压到裁剪边界上，
                // 所以最少让它往右挪半个线宽。
                const float lw = caretW * d;
                const float cx = ox + std::max(caretPos.x, lw * 0.5f);
                const float cy = oy + caretPos.y - scrollY;
                ctx->DrawLine({ cx, cy }, { cx, cy + caretHeight }, caretBrush.Get(), lw);
            }
        }

        ctx->PopAxisAlignedClip();

        // 滚动条：只在内容溢出时画。四边都不留内边距 —— 横向紧贴右边框，
        // 滑轨纵向占满整个控件高度，所以没滚动时滑块贴顶边、滚到底时贴底边。
        if (maxScrollY() > 0.f) {
            const float barW = scrollBarW * d;
            const float left = w - barW;
            ComPtr<ID2D1SolidColorBrush> sbBrush;
            ctx->CreateSolidColorBrush(Color(0x88888833).getD2DColor(), sbBrush.GetAddressOf());
            // 悬停（含拖拽中）时才把滑轨底色画出来，比滑块浅一档。
            if (isHoverScrollBar) {
                ctx->FillRectangle(D2D1::RectF(left, 0.f, w, h), sbBrush.Get());
            }
            sbBrush->SetColor(Color(isHoverScrollBar ? 0x88888888 : 0x88888866).getD2DColor());
            const float top = thumbY();
            auto rc = D2D1::RoundedRect(D2D1::RectF(left, top, w, top + thumbH()),
                barW * 0.5f, barW * 0.5f);
            ctx->FillRoundedRectangle(rc, sbBrush.Get());
        }

        canvas->finishPaint();
    }

    void TextBox::paintSelectionBg(ID2D1DeviceContext* ctx, ID2D1SolidColorBrush* brush)
    {
        if (!textLayout) return;
        const int start = selStart();
        const int end = selEnd();
        // HitTestTextRange 直接给出选区覆盖的所有矩形（自动按行、按双向文本切分），
        // 比自己遍历 GetLineMetrics 逐行推算可靠得多。先问需要多大的数组。
        UINT32 count{ 0 };
        textLayout->HitTestTextRange(start, end - start, 0.f, 0.f, nullptr, 0, &count);
        if (count == 0) return;
        std::vector<DWRITE_HIT_TEST_METRICS> metrics(count);
        if (FAILED(textLayout->HitTestTextRange(start, end - start, 0.f, 0.f,
            metrics.data(), count, &count))) return;
        for (const auto& m : metrics) {
            ctx->FillRectangle(D2D1::RectF(m.left, m.top, m.left + m.width, m.top + m.height), brush);
        }
    }

    // ---- 事件 -----------------------------------------------------------------

    void TextBox::onDown(POINT pos, bool isRight)
    {
        if (isRight) return;
        if (!visual.IsVisible()) return;
        if (!isPosIn(pos)) {
            // 点在控件外：失焦。同窗口内多个 TextBox 的焦点交接就靠这里。
            blur();
            return;
        }
        if (isPosInScrollBar(pos)) {
            // 点滚动条不动光标、也不改焦点态 —— 它不是文本区。
            SetCapture(win->hwnd);
            scrollBarDragging = true;
            isHoverScrollBar = true;
            const float th = thumbH();
            const float trackFree = h - th;
            if (!isPosInThumb(pos) && trackFree > 0.f) {
                // 点在滑轨空白处：先把滑块中心挪到点击点，之后可以接着往下拖。
                setScrollY((pos.y - y - th * 0.5f) / trackFree * maxScrollY());
            }
            dragStartMouseY = (float)pos.y;
            dragStartScrollY = scrollY;
            win->refresh();
            return;
        }
        focus();
        // 捕获鼠标：拖选过程中光标移出控件甚至移出窗口，仍能收到 WM_MOUSEMOVE。
        SetCapture(win->hwnd);
        dragging = true;
        moveCaret(hitTextIndex(pos), (GetKeyState(VK_SHIFT) & 0x8000) != 0);
    }

    void TextBox::onMove(POINT pos)
    {
        if (scrollBarDragging) {
            // 鼠标走过的距离按"滑轨可用行程 : 内容可滚距离"换算成滚动量。
            const float trackFree = h - thumbH();
            if (trackFree > 0.f) {
                setScrollY(dragStartScrollY + (pos.y - dragStartMouseY) / trackFree * maxScrollY());
            }
            return;
        }
        if (dragging) {
            // 拖动中：锚点不动，只移光标 —— 于是选区随鼠标伸缩。
            moveCaret(hitTextIndex(pos), true);
            return;
        }
        if (!visual.IsVisible()) return;
        // mouseLeave 会广播 {INT_MAX, INT_MAX}，isPosIn 自然返回 false，hover 正确复位。
        isHover = isPosIn(pos);
        const bool onBar = isHover && isPosInScrollBar(pos);
        if (onBar != isHoverScrollBar) {
            isHoverScrollBar = onBar;
            win->refresh();
        }
    }

    void TextBox::onUp(POINT pos, bool isRight)
    {
        if (scrollBarDragging) {
            ReleaseCapture();
            scrollBarDragging = false;
            // 松手时鼠标可能已经不在滚动条上了，hover 态要按当前位置重算。
            isHover = isPosIn(pos);
            isHoverScrollBar = isHover && isPosInScrollBar(pos);
            win->refresh();
            return;
        }
        if (!dragging) return;
        ReleaseCapture();
        dragging = false;
        caretVisible = true;
        win->refresh();
    }

    void TextBox::onWheel(POINT pos, float space)
    {
        if (!visual.IsVisible()) return;
        if (!isPosIn(pos)) return;
        if (maxScrollY() <= 0.f) return;
        // space 是像素量，上滚为正 -> 内容向下走 -> scrollY 减小。
        setScrollY(scrollY - space);
    }

    void TextBox::onCharInput(UINT code)
    {
        if (!focused) return;
        // 控制字符与退格交给 onKey 处理（Tab 也在那边统一转成空格）。
        if (code < 0x20 || code == 0x7F) return;
        wchar_t chars[2]{ (wchar_t)code, 0 };
        int len = 1;
        if (code > 0xFFFF) {
            // 拆代理对，From http://unicode.org/faq/utf_bom.html#35
            chars[0] = wchar_t(0xD800 + (code >> 10) - (0x10000 >> 10));
            chars[1] = wchar_t(0xDC00 + (code & 0x3FF));
            len = 2;
        }
        insertText(std::wstring(chars, len));
    }

    void TextBox::onKey(UINT key)
    {
        if (!focused) return;
        const bool isCtrl = (GetKeyState(VK_CONTROL) & 0x8000) != 0;
        const bool isShift = (GetKeyState(VK_SHIFT) & 0x8000) != 0;
        const int len = (int)text.length();

        if (isCtrl) {
            if (key == 'A') {
                selectAll();
            }
            else if (key == 'C' || key == 'X') {
                if (!hasSelection()) return;
                Util::setTextToClipboard(text.substr(selStart(), selEnd() - selStart()));
                if (key == 'X') delSelection();
            }
            else if (key == 'V') {
                auto str = Util::getTextFromClipboard();
                // 剪贴板里的 \r\n 归一成 \n，否则 DWrite 会多折一行出来。
                std::wstring normalized;
                normalized.reserve(str.size());
                for (size_t i = 0; i < str.size(); ++i) {
                    if (str[i] == L'\r') {
                        if (i + 1 < str.size() && str[i + 1] == L'\n') continue;
                        normalized.push_back(L'\n');
                    }
                    else {
                        normalized.push_back(str[i]);
                    }
                }
                insertText(normalized);
            }
            return;
        }

        if (key == VK_RETURN) {
            insertText(L"\n");
        }
        else if (key == VK_TAB) {
            insertText(std::wstring(tabSpaces, L' '));
        }
        else if (key == VK_BACK) {
            if (delSelection()) return;
            if (caretIndex == 0) return;
            // \r\n 当一个字符删。
            int step = 1;
            if (caretIndex >= 2 && text[caretIndex - 1] == L'\n' && text[caretIndex - 2] == L'\r') step = 2;
            text.erase(caretIndex - step, step);
            caretIndex -= step;
            selAnchor = caretIndex;
            buildLayout();
            resetCaretPos(caretIndex);
            scrollCaretIntoView();
            win->refresh();
            onTextChanged(this, text);
        }
        else if (key == VK_DELETE) {
            if (delSelection()) return;
            if (caretIndex >= len) return;
            int step = 1;
            if (caretIndex + 1 < len && text[caretIndex] == L'\r' && text[caretIndex + 1] == L'\n') step = 2;
            text.erase(caretIndex, step);
            buildLayout();
            resetCaretPos(caretIndex);
            win->refresh();
            onTextChanged(this, text);
        }
        else if (key == VK_LEFT) {
            // 无 shift 且有选区时，左键收缩到选区左端而不是单步左移 —— 标准编辑器行为。
            if (!isShift && hasSelection()) moveCaret(selStart(), false);
            else moveCaret(caretIndex - 1, isShift);
        }
        else if (key == VK_RIGHT) {
            if (!isShift && hasSelection()) moveCaret(selEnd(), false);
            else moveCaret(caretIndex + 1, isShift);
        }
        else if (key == VK_UP || key == VK_DOWN) {
            if (!textLayout) return;
            // 用当前光标的 x 去命中上/下一行的 y。这样连续上下移动能保持横向位置。
            const float targetY = key == VK_UP ? caretPos.y - caretHeight * 0.5f
                                               : caretPos.y + caretHeight * 1.5f;
            BOOL isTrailingHit{ FALSE }, isInside{ FALSE };
            DWRITE_HIT_TEST_METRICS hm{};
            textLayout->HitTestPoint(caretPos.x, targetY, &isTrailingHit, &isInside, &hm);
            int index = (int)hm.textPosition;
            if (isTrailingHit) index += (int)hm.length;
            moveCaret(index, isShift);
        }
        else if (key == VK_HOME) {
            if (!textLayout) return;
            BOOL isTrailingHit{ FALSE }, isInside{ FALSE };
            DWRITE_HIT_TEST_METRICS hm{};
            textLayout->HitTestPoint(0.f, caretPos.y, &isTrailingHit, &isInside, &hm);
            moveCaret((int)hm.textPosition, isShift);
        }
        else if (key == VK_END) {
            if (!textLayout) return;
            BOOL isTrailingHit{ FALSE }, isInside{ FALSE };
            DWRITE_HIT_TEST_METRICS hm{};
            // 用一个必然超出行宽的 x 命中行尾。
            textLayout->HitTestPoint(FLT_MAX, caretPos.y, &isTrailingHit, &isInside, &hm);
            int index = (int)hm.textPosition;
            if (isTrailingHit) index += (int)hm.length;
            moveCaret(index, isShift);
        }
        else if (key == VK_ESCAPE) {
            blur();
        }
    }

    void TextBox::onImeStart()
    {
        if (!focused) return;
        HIMC himc = ImmGetContext(win->hwnd);
        if (!himc) return;
        const float d = win->dpi;
        auto [pl, pt, pr, pb] = getPadding();
        // 组字窗口 / 候选框的位置是相对宿主窗口客户区的，所以要把光标的内容坐标
        // 换算回窗口坐标：内容原点 = 控件原点 + padding，再减掉滚动量。
        const float cx = std::min(x + pl * d + caretPos.x, win->w - 1.f);
        const float cyTop = y + pt * d + caretPos.y - scrollY;
        const float cyBottom = std::min(cyTop + caretHeight, win->h - 1.f);

        // 组字窗口贴着光标顶端 —— 组字中的文字应该出现在正在编辑的那一行上。
        COMPOSITIONFORM comp{};
        comp.dwStyle = CFS_FORCE_POSITION;
        comp.ptCurrentPos = { (LONG)cx, (LONG)cyTop };
        ImmSetCompositionWindow(himc, &comp);

        // 候选框用 CFS_EXCLUDE 而不是 CFS_CANDIDATEPOS：后者是让候选框左上角对齐
        // ptCurrentPos，于是候选框会压在正在输入的那一行上（看着就是偏上偏左）。
        // CFS_EXCLUDE 把光标所在的整个矩形交给输入法当排除区，它会把候选框摆到下方。
        CANDIDATEFORM cand{};
        cand.dwStyle = CFS_EXCLUDE;
        cand.ptCurrentPos = { (LONG)cx, (LONG)cyBottom };
        cand.rcArea = { (LONG)cx, (LONG)cyTop, (LONG)(cx + caretW * d), (LONG)cyBottom };
        ImmSetCandidateWindow(himc, &cand);
        ImmReleaseContext(win->hwnd, himc);
    }

    void TextBox::onCursorEvent(bool* handled)
    {
        // onCursor 不带坐标，所以 hover 态必须在 onMove 里自己缓存。
        // 拖滑块时鼠标可能已经移出控件，这时也得继续按滚动条来（别闪回 I 形光标）。
        if (!visual.IsVisible()) return;
        if (!isHover && !scrollBarDragging) return;
        // 滚动条不是文本区，要用箭头而不是 I 形光标。
        SetCursor(LoadCursor(nullptr, (isHoverScrollBar || scrollBarDragging) ? IDC_ARROW : IDC_IBEAM));
        *handled = true;
    }

    void TextBox::onTick(UINT id)
    {
        if (id != timerId || !focused) return;
        // 拖选过程中让光标保持常亮，闪烁会干扰视觉。
        if (dragging) return;
        caretVisible = !caretVisible;
        win->refresh();
    }
}
