
#include "twCamera.h"

namespace TwinkleGraphics
{

Frustum::Frustum(float32 fov, float32 aspect, float32 near, float32 far, FrustumType type)
{
    _projection_matrix = glm::perspective(fov, aspect, near, far);
}

Frustum::Frustum(float32 left, float32 right, float32 bottom, float32 top, float32 near, float32 far, FrustumType type)
{
    if (type == FrustumType::PERSPECTIVE)
    {
        _projection_matrix = glm::frustum(left, right, bottom, top, near, far);
    }
    else
    {
        _projection_matrix = glm::ortho(left, right, bottom, top, near, far);
    }
}

Camera::Camera(Viewport viewport, float32 fov, float32 near, float32 far, FrustumType type)
    : Frustum(fov, viewport.AspectRatio(), near, far, type)
    , _viewport(viewport)
{
}

Camera::~Camera()
{
}

} // namespace TwinkleGraphics
