
#ifndef TW_ORBITCONTROL_H
#define TW_ORBITCONTROL_H

#include "twCamera.h"

namespace TwinkleGraphics
{
class OrbitControl
{
public:
    OrbitControl(Camera::Ptr camera);
    ~OrbitControl();

    void SetMaxDistance(float32 max_distance) { _max_distance = max_distance; _dirty = true; }
    void SetMinDistance(float32 min_distance) { _min_distance = min_distance; _dirty = true; }
    void SetDistance(float32 distance) { _distance = distance; _dirty = true; }
    void SetCenter(glm::vec3 center) { _center = center; _dirty = true; }
    void SetZoomSpeed(float32 speed) { _zoom_speed = speed; _dirty = true; }
    void SetRadius(float32 radius) { _radius = radius; _dirty = true; }

    Camera::Ptr GetCamera() { return _camera; }
    void UpdateCamera();

    void Zoom(float32 factor);
    void Pan(glm::vec3 move);
    void Trackball(glm::vec2 p1, glm::vec2 p2);
    void Trackball(glm::vec2 p);

private:
    void Initialize();

private:
    Camera::Ptr _camera;

    glm::vec3 _center;
    float32 _max_distance;
    float32 _min_distance;
    float32 _distance;
    float32 _zoom_speed;

    float32 _radius;

    bool _dirty;
};
} // namespace TwinkleGraphics

#endif