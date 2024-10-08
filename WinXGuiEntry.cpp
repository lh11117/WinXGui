#include "WinXGuiEntry.h"


int wxg::Entry::style() {
    return (MultiLine?ES_MULTILINE:0) | (Password?ES_PASSWORD:0) | (ReadOnly?ES_READONLY:0) | (ScrollBar?(MultiLine?WS_VSCROLL:0)|WS_HSCROLL:0) | (AutoScroll?(MultiLine? ES_AUTOVSCROLL:0)|ES_AUTOHSCROLL:0);
}

void wxg::Entry::restyle()
{
    wxg::Debug("style: MultiLine=%d, Password=%d, ReadOnly=%d, ScrollBar=%d, AutoScroll=%d, style=%d\n", MultiLine, Password, ReadOnly, ScrollBar, AutoScroll, style());
    SetWindowLong(hWnd, GWL_STYLE, WS_CHILD | WS_VISIBLE | WS_TABSTOP | style());
}

wxg::Entry::Entry(wxg::Window* parent, wxg::WinPos pos_) : WinXGui::Widget(L"", pos_) {
    hWnd = CreateWindow(TEXT("edit"), NULL, WS_CHILD | WS_VISIBLE | WS_TABSTOP | style(), pos_.x, pos_.y, pos_.w, pos_.h, parent->hwnd(), wid, GetModuleHandle(0), NULL);
    RegisterWidget(hWnd);
    font.setHWND(hWnd);
    font.font();
}

wxg::Entry::Entry(wxg::Window* parent, wxg::WinPos pos_, BOOL valueMultiLine, BOOL valuePassword, BOOL valueReadOnly, BOOL valueScrollBar, BOOL valueAutoScroll, int valueMaxLength, LPCWSTR valueText)
    : WinXGui::Widget(L"", pos_)
{
    MultiLine = valueMultiLine;
    Password = valuePassword;
    ReadOnly = valueReadOnly;
    ScrollBar = valueScrollBar;
    AutoScroll = valueAutoScroll;
    MaxLength = valueMaxLength;
    hWnd = CreateWindow(TEXT("edit"), valueText, WS_CHILD | WS_VISIBLE | WS_TABSTOP | style(), pos_.x, pos_.y, pos_.w, pos_.h, parent->hwnd(), wid, GetModuleHandle(0), NULL);
    RegisterWidget(hWnd);
    font.setHWND(hWnd);
    font.font();
}

void wxg::Entry::SetMultiLine(BOOL value) { MultiLine = value; restyle(); }
void wxg::Entry::SetPassword(BOOL value) { Password = value; restyle(); }
void wxg::Entry::SetReadOnly(BOOL value) { ReadOnly = value; restyle(); SendMessage(hWnd, EM_SETREADONLY, value, 0); }
void wxg::Entry::SetScrollBar(BOOL value) { ScrollBar = value; restyle(); }
void wxg::Entry::SetAutoScroll(BOOL value) { AutoScroll = value; restyle(); }
void wxg::Entry::SetMaxLength(int value) { MaxLength = value; restyle(); }
BOOL wxg::Entry::IsMultiLine() const { return MultiLine; }
BOOL wxg::Entry::IsPassword() const { return Password; }
BOOL wxg::Entry::IsReadOnly() const { return ReadOnly; }
BOOL wxg::Entry::IsScrollBar() const { return ScrollBar; }
BOOL wxg::Entry::IsAutoScroll() const { return AutoScroll; }
int wxg::Entry::GetMaxLength() const { return MaxLength; }

void wxg::Entry::SetText(LPCWSTR text)
{
    title = text;
    SetWindowText(hWnd, text);
}

LPCWSTR wxg::Entry::GetText()
{
    int len = GetWindowTextLength(hWnd);
    if (len > 0) {
        LPWSTR text = new WCHAR[len + 1];
        GetWindowText(hWnd, text, len + 1);
        title = text;
        return text;
    }
    return L"";
}

int wxg::Entry::GetTextLength() const
{
    return GetWindowTextLength(hWnd);
}

POINT wxg::Entry::GetCursorPosition() const
{
    DWORD start, end;
    SendMessage(hWnd, EM_GETSEL, (WPARAM)&start, (LPARAM)&end);
    return { (long)start, (long)end };
}

void wxg::Entry::SetCursorPosition(POINT pos)
{
    SendMessage(hWnd, EM_SETSEL, (WPARAM)pos.x, (LPARAM)pos.y);
}

void wxg::Entry::AppendText(LPCWSTR t)
{
    POINT cursorPos = GetCursorPosition();
    int length = GetTextLength();
    int len = wcslen(t);
    wchar_t* text = new wchar_t[length + len + 1];
    wcscpy_s(text, length + len + 1, GetText());
    wcscat_s(text, length + len + 1, t);
    SetText(text);
    delete[] text;
    SetCursorPosition(cursorPos);
}
