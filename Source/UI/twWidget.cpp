#include "twWidget.h"

namespace TwinkleGraphics
{
    Widget::Widget(Widget *parent)
        : Object()
        , _data(new WidgetData)
        , _parent(parent)
    {
    }

    Widget::~Widget()
    {
        SAFE_DEL(_data);
        _parent = nullptr;
    }

    void Widget::Update()
    {}

    void Widget::OnMousePressEvent(MouseEventArgs *e) {}
    void Widget::OnMouseReleaseEvent(MouseEventArgs *e) {}
    void Widget::OnMouseDoubleClickEvent(MouseEventArgs *e) {}
    void Widget::OnMouseMoveEvent(MouseEventArgs *e) {}

    void Widget::OnKeyPressEvent(KeyEventArgs *e) {}
    void Widget::OnKeyReleaseEvent(KeyEventArgs *e) {}

    void Widget::OnFocusInEvent(FocusEventArgs *e) {}
    void Widget::OnFocusOutEvent(FocusEventArgs *e) {}

    void Widget::OnEnterEvent(CursorEventArgs *e) {}
    void Widget::OnLeaveEvent(CursorEventArgs *e) {}

    void Widget::OnResizeEvent(ResizeEventArgs *e) {}
    void Widget::OnCloseEvent(CloseEventArgs *e) {}

} // namespace TwinkleGraphics
