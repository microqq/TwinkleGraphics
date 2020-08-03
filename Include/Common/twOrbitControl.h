
#ifndef TW_ORBITCONTROL_H
#define TW_ORBITCONTROL_H

#include "twCamera.h"

#include "twCameraControl.h"

namespace TwinkleGraphics
{
class OrbitControl : public CameraControl
{
public:
    typedef std::shared_ptr<OrbitControl> Ptr;

    OrbitControl(Camera::Ptr camera);
    virtual ~OrbitControl();

    void SetMaxDistance(float32 max_distance) { _maxDistance = max_distance; _dirty = true; }
    void SetMinDistance(float32 min_distance) { _minDistance = min_distance; _dirty = true; }
    void SetDistance(float32 distance) { _distance = distance; _camera->Translate(glm::vec3(0.0f, 0.0f, _distance)); _dirty = true; }
    void SetCenter(glm::vec3 center) { _center = center; _dirty = true; }
    void SetZoomSpeed(float32 speed) { _zoomSpeed = speed; _dirty = true; }
    void SetRadius(float32 radius) { _radius = radius; _dirty = true; }

    virtual void UpdateCamera() override;

    virtual void HandleMouseLeftButtonDrag(glm::dvec2 p1, glm::dvec2 p2) override 
    {
        this->Trackball(p1, p2);
    }
    virtual void HandleMouseRightButtonDrag(glm::dvec2 p1, glm::dvec2 p2) override 
    {
        this->Pan(p1, p2);
    }
    virtual void HandleMouseScroll(glm::dvec2 scroll) override 
    {
        this->Zoom(scroll);
    }

protected:
    void Initialize();
    virtual void Zoom(glm::vec2 factor);
    virtual void Pan(glm::vec2 p1, glm::vec2 p2);
    virtual void Trackball(glm::vec2 p1, glm::vec2 p2);

    Transform::Ptr _transform;
    Transform::Ptr _target;

    glm::vec3 _center;
    float32 _maxDistance;
    float32 _minDistance;
    float32 _distance;
    float32 _zoomSpeed;

    float32 _radius;

    float32 _rotateX;
    float32 _rotateY;

    bool _dirty;
};
} // namespace TwinkleGraphics

#endif