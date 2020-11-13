#ifndef TW_WIDGET_H
#define TW_WIDGET_H

#include <vector>

#include "twCommon.h"
#include "twMouseEventArgs.h"
#include "twKeyEventArgs.h"
#include "twFocusEventArgs.h"
#include "twResizeEventArgs.h"
#include "twCloseEventArgs.h"
#include "twCursorEventArgs.h"
#include "twSizePolicy.h"
#include "twEventHandler.h"

namespace TwinkleGraphics
{
    struct WidgetData
    {
        SizePolicy sizepolicy;
        uint32 width;
        uint32 height;
        uint32 x;
        uint32 y;
    };

    class Widget : public Object, public IUpdatable, public IDestroyable, public INonCopyable
    {
    public:
        typedef std::function<void()> OnGuiFunction;

        explicit Widget(Widget *parent = nullptr);
        virtual ~Widget();
        virtual void Update(float deltaTime = 0.0f) override;
        virtual void Destroy() override;

        void SetSizePolicy(SizePolicy sizePolicy) { _data->sizepolicy = sizePolicy; }
        void SetPositoin(uint32 x, uint32 y)
        {
            _data->x = x;
            _data->y = y;
        }
        void SetSize(uint32 width, uint32 height) { _data->width = width, _data->height = height; }
        void SetWidth(uint32 width) { _data->width = width; }
        void SetHeight(uint32 height) { _data->height = height; }

        const SizePolicy &GetSizePolicy() { return _data->sizepolicy; }
        vec2 GetPosition() { return vec2(_data->x, _data->y); }
        vec2 GetSize() { return vec2(_data->width, _data->height); }
        uint32 GetWidth() { return _data->width; }
        uint32 GetHeight() { return _data->height; }

        virtual void OnGuiBegin() {}
        virtual void OnGuiEnd() {}

        virtual void OnGui()
        {
            for (auto child : _children)
            {
                if (child != nullptr)
                {
                    child->PaintGui();
                }
            }
        }

        void PaintGui()
        {
            OnGuiBegin();
            OnGui();
            OnGuiEnd();
        }

        void Show() { _visible = true; }
        void Hide() { _visible = false; }

        bool IsFocused() { return _focused; }
        bool IsHovered() { return _hovered; }

        void InstallEventHandler()
        {
            if (_eventHandlerFunc == nullptr)
            {
                _eventHandlerFunc = MakeClassEventHandlerFunPtr(
                    &Widget::OnEvent,
                    this);
                _eventHandler += _eventHandlerFunc;

                Subscribe();
            }
        }

        void UnInstallEventHandler()
        {
            if (_eventHandlerFunc != nullptr)
            {
                _eventHandler -= _eventHandlerFunc;
                UnSubscribe();
            }
        }

        void SetParent(Widget *parent = nullptr);

    protected:
        virtual void OnEvent(Object::Ptr sender, BaseEventArgs::Ptr event);

        virtual void OnMousePressEvent(MouseEventArgs *e);
        virtual void OnMouseReleaseEvent(MouseEventArgs *e);
        virtual void OnMouseDoubleClickEvent(MouseEventArgs *e);
        virtual void OnMouseMoveEvent(MouseEventArgs *e);

        virtual void OnKeyPressEvent(KeyEventArgs *e);
        virtual void OnKeyReleaseEvent(KeyEventArgs *e);

        virtual void OnFocusInEvent(FocusEventArgs *e);
        virtual void OnFocusOutEvent(FocusEventArgs *e);

        virtual void OnEnterEvent(CursorEventArgs *e);
        virtual void OnLeaveEvent(CursorEventArgs *e);

        virtual void OnResizeEvent(ResizeEventArgs *e);
        virtual void OnCloseEvent(CloseEventArgs *e);

        virtual void SetFocusedInternal() {}
        virtual void SetHoveredInternal() {}

        bool HasChild(Widget *widget);
        void AddChild(Widget *widget);
        void RemoveChild(Widget *Widget);

        void SetDepth(int depth = -1) { _depth = depth; }
        int GetDepth() { return _depth; }

        void Subscribe();
        void UnSubscribe();

    protected:
        EventHandler _eventHandler;
        EventHandlerFunctionPtr _eventHandlerFunc = nullptr;
        std::vector<Widget *> _children;
        WidgetData *_data = nullptr;
        Widget *_parent = nullptr;
        int _depth = -1;
        bool _visible = true;
        bool _focused = false;
        bool _hovered = false;
    };
} // namespace TwinkleGraphics

#endif