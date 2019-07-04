

#ifndef TW_FIRSTPERSONCONTROL_H
#define TW_FIRSTPERSONCONTROL_H

#include "twCamera.h"

namespace TwinkleGraphics
{
class FirstPersonControl
{
public:
    FirstPersonControl(Camera::Ptr camera);
    ~FirstPersonControl();

private:
    Camera::Ptr _camera;
}; 
} // namespace TwinkleGraphics


#endif