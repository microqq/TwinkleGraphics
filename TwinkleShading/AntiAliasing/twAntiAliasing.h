


#ifndef TW_TEXTUREEXPLORE_H
#define TW_TEXTUREEXPLORE_H

#include "twCommon.h"

#include "twPlugin.h"
#include "twShader.h"
#include "twMesh.h"
#include "twCamera.h"
#include "twOrbitControl.h"

namespace TwinkleGraphics
{
class AntiAliasingView;
class AntiAliasing : public GLPlugin
{
public:
    AntiAliasing(std::string& name);
    ~AntiAliasing();

    virtual void Install() override;
    virtual void UnInstall() override;

private:
    AntiAliasingView* _view;
    CameraControl* _camera_control;
};

class AntiAliasingView : public View
{
public:
    AntiAliasingView()
        : View()
        {}

    virtual ~AntiAliasingView()
    {}

protected:
    virtual void Initialize() override;
    virtual void Destroy() override;
    virtual void Advance(float64 delta_time) override;
    virtual void RenderImpl() override;
    virtual void OnGUI() override;

    friend class AntiAliasing;
};

}

#endif