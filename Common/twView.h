
#ifndef TW_VIEW_H
#define TW_VIEW_H


// #include <glew/glew.h>
// //include gl after glew
// #include <GL/gl.h>

// #include "twCommon.h"

#include "twCamera.h"

#include "twCameraControl.h"

namespace TwinkleGraphics
{
class View
{
public:
    View();
    ~View();

    void SetViewCamera(Camera::Ptr camera) { _camera = camera; }
    void SetCameraControl(CameraControl* control) { _camera_control = control; }

    const Viewport& GetViewport() 
    { return _camera->GetViewport(); }

    void Resize(float32 scale_x, float32 scale_y)
    {
        _camera->ResizeViewport(scale_x, scale_y);
    }

    void Reset(Rect rect)
    {
        _camera->SetViewportRect(rect);
    }

    View& operator = (const Viewport& viewport);
    View& operator = (const View& view);

    virtual void HandleMouseMove(glm::dvec2 move) {}
    virtual void HandlerMouseLeftButtonDrag(glm::dvec2 p1, glm::dvec2 p2) 
    {
        if(_camera_control != nullptr)
            _camera_control->HandlerMouseLeftButtonDrag(p1, p2);
    }
    virtual void HandlerMouseRightButtonDrag(glm::dvec2 p1, glm::dvec2 p2) 
    {
        if(_camera_control != nullptr)
            _camera_control->HandlerMouseRightButtonDrag(p1, p2);
    }
    virtual void HandlerMouseScroll(glm::dvec2 scroll) 
    {
        if(_camera_control != nullptr)
            _camera_control->HandlerMouseScroll(scroll);
    }

    void Run();
    void OnViewGUI() { this->OnGUI(); }

protected:
    virtual void Initialize() {}
    virtual void Advance(float64 delta_time);
    virtual void HandleEvents();
    virtual void RenderImpl() {}
    virtual void Destroy() {}
    virtual void OnGUI() {}

private:
    void Render();

protected:
    Camera::Ptr _camera;
    CameraControl* _camera_control;

    bool _done;
};

} // namespace TwinkleGraphics


#endif