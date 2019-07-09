#ifndef TW_CAMERACONTROL_H
#define TW_CAMERACONTROL_H

#include <memory>

#include "twCamera.h"

namespace TwinkleGraphics
{
class CameraControl
{
public:
    CameraControl(Camera::Ptr camera) : _camera(camera) {}
    virtual ~CameraControl() {}

    void SetCamera(Camera::Ptr camera) { _camera = camera; }
    Camera::Ptr GetCamera() { return _camera; }
    virtual void UpdateCamera() {}

    virtual void HandleMouseMove(glm::dvec2 move) {}
    virtual void HandlerMouseLeftButtonDrag(glm::dvec2 p1, glm::dvec2 p2) {}
    virtual void HandlerMouseRightButtonDrag(glm::dvec2 p1, glm::dvec2 p2) {}
    virtual void HandlerMouseScroll(glm::dvec2 scroll) {}

protected:
    Camera::Ptr _camera;
};
} // namespace TwinkleGraphics


#endif