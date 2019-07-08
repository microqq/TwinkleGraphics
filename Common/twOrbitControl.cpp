

#include <iostream>

#include "twOrbitControl.h"


namespace TwinkleGraphics
{
OrbitControl::OrbitControl(Camera::Ptr camera)
    : _camera(camera)
    , _transform(nullptr)
    , _target(nullptr)
    , _center()
    , _max_distance(100.0f)
    , _min_distance(10.0f)
    , _distance(25.0f)
    , _zoom_speed(1.0f)
    , _radius(10.0f)
{
    Initialize();
}

OrbitControl::~OrbitControl()
{
}

void OrbitControl::Initialize()
{
    if(_target == nullptr)
    {
        _transform = std::make_shared<Transform>();
    }
    else
    {
        _transform = _target;
    }
    _camera->ResetTransform();
    _camera->GetTransform()->SetParent(_transform);
    _camera->Translate(glm::vec3(0.0f, 0.0f, _distance));

    _transform->Translate(glm::vec3(0.0f, 5.0f, 0.0f));
    _transform->Rotate(glm::radians<float32>(70.0f), glm::vec3(0.0f, 1.0f, 0.0f));
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

    glm::vec2 spherical_p1 = p1 / screen_size;
    glm::vec2 spherical_p2 = p2 / screen_size;

    spherical_p1 = (spherical_p1 - glm::vec2(0.5f, 0.5f)) * 2.0f * _radius;
    spherical_p2 = (spherical_p2 - glm::vec2(0.5f, 0.5f)) * 2.0f * _radius;

    glm::vec2 p = spherical_p2 - spherical_p1;
    glm::vec3 v0(0.0f, 0.0f, 1.0f);
    glm::vec3 v1 = glm::normalize(glm::vec3(p.x, p.y, _radius));

    glm::vec3 v = glm::normalize(glm::cross(v0, v1));
    float32 theta = glm::acos(glm::dot(v0, v1));

    if (::fabs(theta) <= glm::epsilon<float32>())
        return;

    _transform->Rotate(theta, v);

    _dirty = true;
}

} // namespace TwinkleGraphics
