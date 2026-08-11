#include "pch.h"
#include "WindowTextBox.h"

WindowTextBox::WindowTextBox() : Ling::WinBase()
{
    setTitle(L"TextBox 多行文本输入演示");
    setSize(820, 620);
    setCenter();
    onDestroy.add([this] { Ling::App::get()->quit(); });
    createNativeWindow();
}

WindowTextBox::~WindowTextBox()
{
}

void WindowTextBox::onCreated()
{
    enableShadow();
    body->setBg(0xFFFFFFFF);
    body->setFlexDirection(Ling::FlexDirection::Column);
    titleBar = std::make_unique<TitleBar>(this);

    auto center = body->makeChild<Ling::Node>();
    center->setFlexGrow(1.f);
    center->setFlexDirection(Ling::FlexDirection::Column);
    center->setAlignItems(Ling::Align::Center);
    center->setPadding(24.f);

    auto tip = center->makeChild<Ling::Label>();
    tip->setText(L"支持：中文输入法、自动换行、回车换行、鼠标拖选、Shift+方向键扩选、Ctrl+A/C/X/V、滚轮滚动");
    tip->setFontSize(13.f);
    tip->setColor(0x888888FF);
    tip->setMarginBottom(12.f);

    // 主编辑框：预置多行文本，内容比可视高度长，用来演示滚动
    editor = center->makeChild<Ling::TextBox>();
    editor->setSize(680.f, 240.f);
    editor->setFontSize(15.f);
    editor->setPadding(10.f);
    editor->setBg(0xFCFCFCFF);
    editor->setBorder(1.f, 0xD9D9D9FF);
    editor->setBorderRadius(6.f);
    editor->setText(
        L"这是一个多行文本输入控件。\n"
        L"文本会按控件宽度自动换行，所以很长的一行也不会跑到框外面去，"
        L"而是老老实实折到下一行继续显示。\n"
        L"\n"
        L"试试这些操作：\n"
        L"    1. 用鼠标拖动选中一段文字，然后直接输入替换掉它\n"
        L"    2. 按住 Shift 配合方向键 / Home / End 扩展选区\n"
        L"    3. Ctrl+A 全选，Ctrl+C 复制，Ctrl+V 粘贴\n"
        L"    4. 切到中文输入法打几个字，看候选框是否跟着光标走\n"
        L"    5. 内容超出高度后滚轮滚动，或者按方向键让光标自动滚入可视区\n"
        L"\n"
        L"最后一行。");

    infoLabel = center->makeChild<Ling::Label>();
    infoLabel->setFontSize(13.f);
    infoLabel->setColor(0x666666FF);
    infoLabel->setMarginTop(8.f);
    editor->onTextChanged.add([this](Ling::TextBox*, const std::wstring&) { updateInfo(); });
    updateInfo();

    auto btnBox = center->makeChild<Ling::Node>();
    btnBox->setFlexDirection(Ling::FlexDirection::Row);
    btnBox->setMarginTop(12.f);

    auto btnSelectAll = btnBox->makeChild<Ling::Button>();
    btnSelectAll->setText(L"全选");
    btnSelectAll->setSize(80.f, 30.f);
    btnSelectAll->setFontSize(14.f);
    btnSelectAll->setBg(0x3B82F6FF);
    btnSelectAll->setColor(0xFFFFFFFF);
    btnSelectAll->setHoverBg(0x2563EBFF);
    btnSelectAll->setHoverColor(0xFFFFFFFF);
    btnSelectAll->setBorderRadius(4.f);
    btnSelectAll->setMarginRight(10.f);
    btnSelectAll->onClick.add([this](Ling::Button*) { editor->selectAll(); });

    auto btnClear = btnBox->makeChild<Ling::Button>();
    btnClear->setText(L"清空");
    btnClear->setSize(80.f, 30.f);
    btnClear->setFontSize(14.f);
    btnClear->setBg(0xF2F2F2FF);
    btnClear->setColor(0x333333FF);
    btnClear->setHoverBg(0xE6E6E6FF);
    btnClear->setHoverColor(0x333333FF);
    btnClear->setBorderRadius(4.f);
    btnClear->onClick.add([this](Ling::Button*) { editor->setText(L""); });

    // 第二个输入框：空内容 + 占位符，同时验证同窗口内两个 TextBox 的焦点交接
    noteBox = center->makeChild<Ling::TextBox>();
    noteBox->setSize(680.f, 90.f);
    noteBox->setFontSize(14.f);
    noteBox->setPadding(10.f);
    noteBox->setMarginTop(16.f);
    noteBox->setBg(0xFCFCFCFF);
    noteBox->setBorder(1.f, 0xD9D9D9FF);
    noteBox->setBorderRadius(6.f);
    noteBox->setPlaceholder(L"这里是占位符，点进来输入试试（两个框的焦点应当互斥）");
    noteBox->setSelectionBgColor(0xFFD59199);
    noteBox->setCaretColor(0xE11D48FF);

    show();
}

void WindowTextBox::updateInfo()
{
    auto len = editor->getText().length();
    infoLabel->setText(std::format(L"字符数：{}", len));
}

LRESULT WindowTextBox::onHitTest(const POINT pos)
{
    POINT pt = pos;
    ScreenToClient(hwnd, &pt);
    if (!isMaximized) {
        auto result = borderHitTest(pt);
        if (result != HTCLIENT) return result;
    }
    return titleBar->hitCaption(pt);
}
