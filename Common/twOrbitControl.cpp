

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
{}


void OrbitControl::Initialize()
{}

void OrbitControl::UpdateCamera()
{
    //translate\rotate...
}


void OrbitControl::Zoom(float32 factor)
{
    _dirty = true;
}

void OrbitControl::Pan(float32 x, float32 y, float32 z)
{
    _dirty = true;
}

void OrbitControl::Trackball(float32 x, float32 y)
{
    _dirty = true;
}

} // namespace TwinkleGraphics
