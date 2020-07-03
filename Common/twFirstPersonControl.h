

#ifndef TW_FIRSTPERSONCONTROL_H
#define TW_FIRSTPERSONCONTROL_H

#include "twCamera.h"

#include "twCameraControl.h"

namespace TwinkleGraphics
{
class FirstPersonControl : public CameraControl
{
public:
    FirstPersonControl(Camera::Ptr camera) : CameraControl(camera) 
    {}
    virtual ~FirstPersonControl() {}

    virtual void UpdateCamera() override {}

private:
}; 
} // namespace TwinkleGraphics


#endif