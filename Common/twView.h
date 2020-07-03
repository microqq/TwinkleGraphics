
#ifndef TW_VIEW_H
#define TW_VIEW_H


// #include <glew/glew.h>
// //include gl after glew
// #include <GL/gl.h>

// #include "twCommon.h"

#include "twCamera.h"
#include "twCameraControl.h"
#include "twSceneManagement.h"

namespace TwinkleGraphics
{
class View
{
public:
    View();
    ~View();

    // [[deprecated]]
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

    // [[deprecated]]
    void SetCameraControl(CameraControl::Ptr control) { _camera_control = control; }

    void AttachScene(Scene::Ptr scene) 
    {
        _scene = scene;
        if(_camera != nullptr)
        {
            _scene->SetMainCamera(_camera);
        }
    }

    const Viewport& GetViewport()
    { return _camera->GetViewport(); }

    void Init(glm::ivec4 rect)
    {
        SetViewRect(rect);
        Initialize();
    }

    // [[deprecated]]
    void Init(glm::ivec2 size) 
    {
        SetViewSize(size);
        Initialize();
    }

    void SetViewRect(Rect rect)
    {
        _rect = rect;
    }

    glm::ivec4 GetViewRect() { return _rect; }

    // [[deprecated]]
    void SetViewSize(glm::ivec2 size)
    {
        _rect.z = size.x;
        _rect.w = size.y;
    }

    // [[deprecated]]
    glm::ivec2 GetViewSize() { return ivec2(_rect.z, _rect.w); }

    void Resize(float32 scale_x, float32 scale_y)
    {
        _rect.x = _rect.x * scale_x;
        _rect.y = _rect.y * scale_y;
        _camera->ResizeViewport(scale_x, scale_y);
    }

    void ResetViewport(const Rect& rect)
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
    // [[deprecated]]
    virtual void RenderImpl() {}
    virtual void Destroy() {}
    virtual void OnGUI() {}

private:
    void Render();

protected:
    // [[deprecated]]
    Camera::Ptr _camera;
    // [[deprecated]]
    CameraControl::Ptr _camera_control;

    Scene::Ptr _scene;

    glm::ivec4 _rect;

    bool _done;
    bool _initialized;
};

} // namespace TwinkleGraphics


#endif