#include <algorithm>

#include "twWidget.h"
#include "twConsoleLog.h"
#include "twEventManager.h"

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

        UnInstallEventHandler();
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

    void Widget::OnEvent(Object::Ptr sender, BaseEventArgs::Ptr event)
    {
        InputEventArgs::Ptr inputEventPtr = std::dynamic_pointer_cast<InputEventArgs>(event);
        if(inputEventPtr != nullptr)
        {
            InputEventArgs *event = inputEventPtr.get();
            
            MouseEventArgs* mouseEvent = dynamic_cast<MouseEventArgs*>(event);
            if (mouseEvent != nullptr)
            {
            }

            KeyEventArgs* keyEvent = dynamic_cast<KeyEventArgs*>(event);
            if (keyEvent != nullptr)
            {
            }

            CursorEventArgs* cursorEvent = dynamic_cast<CursorEventArgs*>(event);
            if (cursorEvent != nullptr)
            {
            }

            ResizeEventArgs* resizeEvent = dynamic_cast<ResizeEventArgs*>(event);
            if (resizeEvent != nullptr)
            {
                OnResizeEvent(resizeEvent);
            }

            CloseEventArgs* closeEvent = dynamic_cast<CloseEventArgs*>(event);
            if (closeEvent != nullptr)
            {
                OnCloseEvent(closeEvent);
            }
        }
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
            int parDepth = _depth;
            widget->SetDepth(parDepth + 1);
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
            std::vector<Widget*>::iterator iter = _children.erase(std::remove_if(_children.begin(), _children.end()
            , [widget](Widget* w)
            {
                return w == widget;
            }));

            
        }
    }

    void Widget::Subscribe()
    {
        EventManager &eventMgrInst = EventMgrInstance();
        eventMgrInst.Subscribe(InputEventArgs::ID, _eventHandler);
    }

    void Widget::UnSubscribe()
    {
        EventManager &eventMgrInst = EventMgrInstance();
        eventMgrInst.UnSubscribe(InputEventArgs::ID, _eventHandler);
    }
} // namespace TwinkleGraphics
