
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

    void SetMaxDistance(float32 max_distance) { _max_distance = max_distance; _dirty = true; }
    void SetMinDistance(float32 min_distance) { _min_distance = min_distance; _dirty = true; }
    void SetDistance(float32 distance) { _distance = distance; _dirty = true; }
    void SetCenter(glm::vec3 center) { _center = center; _dirty = true; }
    void SetZoomSpeed(float32 speed) { _zoom_speed = speed; _dirty = true; }
    void SetRadius(float32 radius) { _radius = radius; _dirty = true; }

    virtual void UpdateCamera() override;

    virtual void HandlerMouseLeftButtonDrag(glm::dvec2 p1, glm::dvec2 p2) override 
    {
        this->Trackball(p1, p2);
    }
    virtual void HandlerMouseRightButtonDrag(glm::dvec2 p1, glm::dvec2 p2) override 
    {
        this->Pan(p1, p2);
    }
    virtual void HandlerMouseScroll(glm::dvec2 scroll) override 
    {
        this->Zoom(scroll);
    }

private:
    void Initialize();
    void Zoom(glm::vec2 factor);
    void Pan(glm::vec2 p1, glm::vec2 p2);
    void Trackball(glm::vec2 p1, glm::vec2 p2);

private:
    Transform::Ptr _transform;
    Transform::Ptr _target;

    glm::vec3 _center;
    float32 _max_distance;
    float32 _min_distance;
    float32 _distance;
    float32 _zoom_speed;

    float32 _radius;

    float32 _rotateX;
    float32 _rotateY;

    bool _dirty;
};
} // namespace TwinkleGraphics

#endif