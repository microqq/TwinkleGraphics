


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
class TextureExploreView;
class TextureExplore : public GLPlugin
{
public:
    TextureExplore(std::string& name);
    ~TextureExplore();

    virtual void Install() override;
    virtual void UnInstall() override;

private:
    TextureExploreView* _view;
    CameraControl* _camera_control;
};

class TextureExploreView : public View
{
public:
    TextureExploreView()
        : View()
        , _current_tex_option(-1)
        {}

    virtual ~TextureExploreView()
    {}

protected:
    virtual void Initialize() override;
    virtual void Destroy() override;
    virtual void Advance(float64 delta_time) override;
    virtual void RenderImpl() override;
    virtual void OnGUI() override;

private:
    

private:
    int32 _current_tex_option;

    friend class TextureExplore;
};

}

#endif