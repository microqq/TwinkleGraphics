
#include "twCamera.h"

namespace TwinkleGraphics
{

Frustum::Frustum(float32 fov, float32 aspect, float32 near, float32 far, FrustumType type)
    : _frustum_type(type)
    , _fov(fov)
    , _aspect(aspect)
    , _near(near)
    , _far(far)
{
    _projection_matrix = glm::perspective(fov, aspect, near, far);
}

Frustum::Frustum(float32 left, float32 right, float32 bottom, float32 top, float32 near, float32 far, FrustumType type)
    : _frustum_type(type)
{
    //compute _fov\_aspect....

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
    : Object()
    , Frustum(fov, viewport.AspectRatio(), near, far, type)
    , ISceneNode()
    , _viewport(viewport)
    , _viewport_dirty(true)
    , _rendertoraget(false)
{
}

Camera::~Camera()
{
}

} // namespace TwinkleGraphics
