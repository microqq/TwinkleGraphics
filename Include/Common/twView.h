
#ifndef TW_VIEW_H
#define TW_VIEW_H


// #include <glew/glew.h>
// //include gl after glew
// #include <GL/gl.h>

#include "twCamera.h"
#include "twCameraControl.h"
#include "twSceneManager.h"

namespace TwinkleGraphics
{
class __TWCOMExport View
{
public:
    View();
    ~View();

    // [[deprecated]]
    void SetViewCamera(CameraPtr camera) 
    { 
        if(camera == nullptr)
            return;
        _camera = camera;
    }

    // [[deprecated]]
    void SetCameraControl(CameraControlPtr control) { _cameraControl = control; }

    void AttachScene(Scene::Ptr scene) 
    {
        if(scene == nullptr)
        {
            return;
        }
        _scene = scene;

        if(_camera == nullptr)
        {
            _camera = _scene->GetMainCamera();
        }
    }

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

    void ResetViewRect(const Rect& rect)
    {
        _rect.x = rect.x;        
        _rect.y = rect.y;        
        _rect.z = rect.z;        
        _rect.w = rect.w;        
        _camera->SetViewportRect(rect);
    }

    View& operator = (const Viewport& viewport);
    View& operator = (const View& view);

    virtual void HandleMouseMove(glm::dvec2 move) {}
    virtual void HandleMouseLeftButtonDrag(glm::dvec2 p1, glm::dvec2 p2) 
    {
        if(_cameraControl != nullptr)
            _cameraControl->HandleMouseLeftButtonDrag(p1, p2);
    }
    virtual void HandleMouseRightButtonDrag(glm::dvec2 p1, glm::dvec2 p2) 
    {
        if(_cameraControl != nullptr)
            _cameraControl->HandleMouseRightButtonDrag(p1, p2);
    }
    virtual void HandleMouseScroll(glm::dvec2 scroll) 
    {
        if(_cameraControl != nullptr)
            _cameraControl->HandleMouseScroll(scroll);
    }
    virtual void HandleKeyPress(int32 key
        , int32 scannode
        , int32 action
        , int32 mods)
    {
        if(_cameraControl != nullptr)
            _cameraControl->HandleKeyPress(key, scannode, action, mods);
    }
    virtual void HandleKeyRelease(int32 key
        , int32 scannode
        , int32 action
        , int32 mods)
    {
        if(_cameraControl != nullptr)
            _cameraControl->HandleKeyRelease(key, scannode, action, mods);
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
    CameraPtr _camera;
    // [[deprecated]]
    CameraControlPtr _cameraControl;

    Scene::Ptr _scene;

    glm::ivec4 _rect;

    bool _done;
    bool _initialized;
};

} // namespace TwinkleGraphics


#endif