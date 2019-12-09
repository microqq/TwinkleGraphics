

#include <iostream>

#include "twOrbitControl.h"


namespace TwinkleGraphics
{
OrbitControl::OrbitControl(Camera::Ptr camera)
    : CameraControl(camera)
    , _transform(nullptr)
    , _target(nullptr)
    , _center()
    , _max_distance(500.0f)
    , _min_distance(5.0f)
    , _distance(25.0f)
    , _zoom_speed(1.0f)
    , _radius(10.0f)
    , _rotateX(0.0f)
    , _rotateY(0.0f)
    , _dirty(true)
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
    // _camera->LookAt(glm::vec3(0.0f, -10.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    _camera->Translate(glm::vec3(0.0f, 0.0f, _distance));

    // _transform->Translate(_center);
    // _transform->Rotate(glm::radians<float32>(70.0f), glm::vec3(0.0f, 1.0f, 0.0f));
}

void OrbitControl::UpdateCamera()
{
    //translate\rotate...

    if(_dirty)
    {
        _camera->GetViewMatrix();
        _camera->GetProjectionMatrix();

        _dirty = false;
    }
}


void OrbitControl::Zoom(glm::vec2 factor)
{
    float32 dy = factor.y;

    float32 distance = _distance - dy;
    float32 offset = _distance;
    if(distance < _min_distance)
        _distance = _min_distance;
    else if(distance > _max_distance)
        _distance = _max_distance;
    else
    {
        _distance = distance;
    }

    offset = _distance - offset;
    
    _camera->Translate(glm::vec3(0.0f, 0.0f, offset));

    _dirty = true;
}

/**
 * @brief 
 * 
 * @param p1 
 * @param p2 
 */
void OrbitControl::Pan(glm::vec2 p1, glm::vec2 p2)
{
    glm::vec2 screen_size = glm::vec2(_camera->GetViewport().Width()
        , _camera->GetViewport().Height() 
    );

    glm::vec2 spherical_p1 = p1 / screen_size;
    glm::vec2 spherical_p2 = p2 / screen_size;

    spherical_p1 = (spherical_p1 - glm::vec2(0.5f, 0.5f)) * 2.0f * _radius;
    spherical_p2 = (spherical_p2 - glm::vec2(0.5f, 0.5f)) * 2.0f * _radius;
    glm::vec2 p = spherical_p2 - spherical_p1;
    glm::vec3 d = glm::vec3(-p.x, p.y, 0.0f);

    glm::mat3 rot_mat = glm::mat3_cast(_transform->GetLocalToWorldOrientation());
    d = rot_mat * d;

    _transform->Translate(d);

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
    glm::vec3 v1 = glm::normalize(glm::vec3(p.x, 0.0f, _radius));
    glm::vec3 v2 = glm::normalize(glm::vec3(0.0f, p.y, _radius));

    float32 factor = 1.0f;

    float32 theta_x = glm::acos(glm::dot(v0, v1));
    if (::fabs(theta_x) > glm::epsilon<float32>())
    {
        factor = p.x > 0.0f ? 1.0f : -1.0f;
        _rotateX += theta_x * factor;
    }

    float32 theta_y = glm::acos(glm::dot(v0, v2));
    if (::fabs(theta_y) > glm::epsilon<float32>())
    {
        factor = p.y > 0.0f ? -1.0f : 1.0f; 
        _rotateY += theta_y * factor;
    }

    _transform->SetOrientation(glm::identity<glm::quat>());
    _transform->Rotate(glm::vec3(_rotateY, _rotateX, 0.0f));

    _dirty = true;
}

} // namespace TwinkleGraphics
