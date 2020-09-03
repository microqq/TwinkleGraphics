

#ifndef TW_FIRSTPERSONCONTROL_H
#define TW_FIRSTPERSONCONTROL_H

#include "twCamera.h"

#include "twOrbitControl.h"

namespace TwinkleGraphics
{
class __TWCOMExport FirstPersonControl : public OrbitControl
{
public:
    typedef std::shared_ptr<FirstPersonControl> Ptr;

    FirstPersonControl(Camera::Ptr camera); 
    virtual ~FirstPersonControl();

    void SetStep(float stepx, float stepz)
    {
        _stepX = stepx;
        _stepZ = stepz;
    }

    virtual void HandleKeyPress(int32 key
        , int32 scannode
        , int32 action
        , int32 mods) override;
    virtual void HandleKeyRelease(int32 key
        , int32 scannode
        , int32 action
        , int32 mods) override;

protected:

private:
    float _stepX; 
    float _stepZ;
}; 
} // namespace TwinkleGraphics


#endif