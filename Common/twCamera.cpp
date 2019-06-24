
#include "twCamera.h"

namespace TwinkleGraphics
{

Camera::Camera(Viewport viewport, float32 fov, float32 near, float32 far, FrustumType type)
    : Frustum(fov, viewport.AspectRatio(), near, far, type)
    , _viewport(viewport)
{
}

Camera::~Camera()
{
}

} // namespace TwinkleGraphics
