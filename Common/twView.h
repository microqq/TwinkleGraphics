
#ifndef TW_VIEW_H
#define TW_VIEW_H


// #include <glew/glew.h>
// //include gl after glew
// #include <GL/gl.h>

// #include "twCommon.h"

#include "twCamera.h"
#include "twCameraControl.h"
#include "twScene.h"

namespace TwinkleGraphics
{
class View
{
public:
    View();
    ~View();

    void SetViewCamera(Camera::Ptr camera) 
    { 
        if(camera == nullptr)
            return;
        _camera = camera;

        if(_scene != nullptr)
        {
            _scene->SetMainCamera(_camera);
        }
    }
    void SetCameraControl(CameraControl::Ptr control) { _camera_control = control; }
    void AttachToScene(Scene::Ptr scene) 
    {
        _scene = scene;
        if(_camera != nullptr)
        {
            _scene->SetMainCamera(_camera);
        }
    }

    const Viewport& GetViewport()
    { return _camera->GetViewport(); }

    void Init(glm::ivec2 size) 
    {
        SetWindowSize(size);
        Initialize();
    }
    void SetWindowSize(glm::ivec2 size)
    {
        _window_size = size;
    }
    const glm::ivec2& GetWindowSize() { return _window_size; }

    void Resize(float32 scale_x, float32 scale_y)
    {
        _window_size.x = _window_size.x * scale_x;
        _window_size.y = _window_size.y * scale_y;
        _camera->ResizeViewport(scale_x, scale_y);
    }

    void ResetViewport(Rect rect)
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
    virtual void Initialize() { _initialized = true; }
    virtual void Advance(float64 delta_time);
    virtual void HandleEvents();
    virtual void RenderImpl() {}
    virtual void Destroy() {}
    virtual void OnGUI() {}

private:
    void Render();

protected:
    Camera::Ptr _camera;
    CameraControl::Ptr _camera_control;
    Scene::Ptr _scene;

    glm::ivec2 _window_size;

    bool _done;
    bool _ismainview;
    bool _initialized;
};

} // namespace TwinkleGraphics


#endif