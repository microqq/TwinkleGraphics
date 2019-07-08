

#include "twOrbitControl.h"

namespace TwinkleGraphics
{
OrbitControl::OrbitControl(Camera::Ptr camera)
    : _camera(camera)
    , _center()
    , _max_distance(50.0f)
    , _min_distance(2.0f)
    , _distance(5.0f)
    , _zoom_speed(1.0f)
    , _radius(1.0f)
{}

OrbitControl::~OrbitControl()
{
}

void OrbitControl::Initialize()
{
    _camera->ResetTransform();
}

void OrbitControl::UpdateCamera()
{
    //translate\rotate...
}


void OrbitControl::Zoom(float32 factor)
{
    _dirty = true;
}

void OrbitControl::Pan(glm::vec3 move)
{
    _dirty = true;
}

/**
 * @brief 
 * p1、p2 relative to screen space
 * @param p1 
 * @param p2 
 */
void OrbitControl::Trackball(glm::vec2 p1, glm::vec2 p2)
{
    glm::vec2 screen_size = glm::vec2(_camera->GetViewport().Width()
        , _camera->GetViewport().Height() 
    );
    glm::vec2 p = p2 - p1;
    p /= screen_size;
    p = (p - glm::vec2(0.5f, 0.5f)) * 2.0f * _radius;

    glm::vec3 v0(0.0f, 0.0f, 1.0f);
    glm::vec3 v1 = glm::normalize(glm::vec3(p.x, p.y, _radius));

    glm::vec3 v = glm::normalize(glm::cross(v0, v1));
    float32 theta = glm::acos(glm::dot(v0, v1));
    _camera->Rotate(theta, v);

    _dirty = true;
}

void OrbitControl::Trackball(glm::vec2 p)
{
    glm::vec2 screen_size = glm::vec2(_camera->GetViewport().Width()
        , _camera->GetViewport().Height() 
    );
    p /= screen_size;
    p = (p - glm::vec2(0.5f, 0.5f)) * 2.0f * _radius;

    glm::vec3 v0(0.0f, 0.0f, 1.0f);
    glm::vec3 v1 = glm::normalize(glm::vec3(p.x, p.y, _radius));

    glm::vec3 v = glm::normalize(glm::cross(v0, v1));
    float32 theta = glm::acos(glm::dot(v0, v1));
    _camera->Rotate(theta, v);

    _dirty = true;
}

} // namespace TwinkleGraphics
