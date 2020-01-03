
#include "twCamera.h"

namespace TwinkleGraphics
{

Frustum::Frustum(float32 fov, float32 aspect, float32 near, float32 far)
    : _frustum_type(FrustumType::PERSPECTIVE)
    , _fov(fov)
    , _aspect(aspect)
    , _near(near)
    , _far(far)
{
    _projection_matrix = glm::perspective(fov, aspect, near, far);
}

Frustum::Frustum(float32 left, float32 right, float32 bottom, float32 top, float32 near, float32 far)
    : _frustum_type(FrustumType::ORTHOGRAPHIC)
    , _aspect((right - left) / (top - bottom))
    , _near(near)
    , _far(far)
{
    _projection_matrix = glm::ortho(left, right, bottom, top, near, far);
}

Camera::Camera(Viewport viewport, float32 fov, float32 near, float32 far)
    : Object()
    , Frustum(fov, viewport.AspectRatio(), near, far)
    , ISceneNode()
    , _viewport(viewport)
    , _cullingmask((int32)CullingMask::EVERYTHING)
    , _sortdepth(-1)
    , _viewport_dirty(true)
    , _rendertoraget(false)
{
}

Camera::Camera(Viewport viewport, float32 near, float32 far)
    : Object()
    , Frustum(-viewport.Width() * 0.5f, viewport.Width() * 0.5f, -viewport.Height() * 0.5f, viewport.Height() * 0.5f, near, far)
    , ISceneNode()
    , _viewport(viewport)
    , _cullingmask((int32)CullingMask::EVERYTHING)
    , _sortdepth(-1)
    , _viewport_dirty(true)
    , _rendertoraget(false)
{
}

Camera::~Camera()
{
}

} // namespace TwinkleGraphics
