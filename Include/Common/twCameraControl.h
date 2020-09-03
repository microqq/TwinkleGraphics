#ifndef TW_CAMERACONTROL_H
#define TW_CAMERACONTROL_H

#include <memory>

#include "twCamera.h"

namespace TwinkleGraphics
{
class __TWCOMExport CameraControl : public Object
{
public:
    typedef std::shared_ptr<CameraControl> Ptr;

    CameraControl(Camera::Ptr camera) : _camera(camera) {}
    virtual ~CameraControl() {}

    void SetCamera(Camera::Ptr camera) { _camera = camera; }
    Camera::Ptr GetCamera() { return _camera; }
    virtual void UpdateCamera() {}

    virtual void HandleMouseMove(glm::dvec2 move) {}
    virtual void HandleMouseLeftButtonDrag(glm::dvec2 p1, glm::dvec2 p2) {}
    virtual void HandleMouseRightButtonDrag(glm::dvec2 p1, glm::dvec2 p2) {}
    virtual void HandleMouseScroll(glm::dvec2 scroll) {}
    virtual void HandleKeyPress(int32 key
        , int32 scannode
        , int32 action
        , int32 mods) {}
    virtual void HandleKeyRelease(int32 key
        , int32 scannode
        , int32 action
        , int32 mods) {}

protected:
    Camera::Ptr _camera;
};
} // namespace TwinkleGraphics


#endif