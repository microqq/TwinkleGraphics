#include <algorithm>

#include "twWidget.h"
#include "twConsoleLog.h"

namespace TwinkleGraphics
{
    Widget::Widget(Widget *parent)
        : Object()
        , IUpdatable()
        , IDestroyable()
        , INonCopyable()
        , _children()
        , _data(new WidgetData)
    {
        SetParent(parent);
        Show();
    }

    Widget::~Widget()
    {
        Destroy();
    }

    void Widget::Update(float deltaTime)
    {
        for(auto w : _children)
        {
            w->Update(deltaTime);
        }
    }

    void Widget::Destroy()
    {
        SAFE_DEL(_data);
        _parent = nullptr;

        for(auto w : _children)
        {
            w->Destroy();
            SAFE_DEL(w);
        }
        _children.clear();
    }

    void Widget::SetParent(Widget* parent)
    {
        if(parent == nullptr || _parent == parent)
        {
            return;
        }
        
        if(_parent != nullptr)
        {
            _parent->RemoveChild(this);
        }
        _parent = parent;
        _parent->AddChild(this);
    }

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

    void Widget::OnResizeEvent(ResizeEventArgs *e) 
    {
        
    }

    void Widget::OnCloseEvent(CloseEventArgs *e) {}

    bool Widget::HasChild(Widget* widget)
    {
        using Iter = std::vector<Widget*>::iterator;
        Iter find = std::find_if(_children.begin(), _children.end()
        , [widget](Widget* w)
        {
            return w == widget;
        });

        return find != _children.end();
    }

    void Widget::AddChild(Widget* widget)
    {
        if(widget == nullptr)
        {
            return;
        }

        if(!HasChild(widget))
        {
            _children.emplace_back(widget);
        }
    }

    void Widget::RemoveChild(Widget* widget)
    {
        if(widget == nullptr)
        {
            return;
        }

        if(HasChild(widget))
        {
            _children.erase(std::remove_if(_children.begin(), _children.end()
            , [widget](Widget* w)
            {
                return w == widget;
            }));
        }
    }
} // namespace TwinkleGraphics
