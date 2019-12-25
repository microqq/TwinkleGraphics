


#ifndef TW_TEXTUREEXPLORE_H
#define TW_TEXTUREEXPLORE_H

#include "twCommon.h"

#include "twPlugin.h"
#include "twShader.h"
#include "twMesh.h"
#include "twCamera.h"
#include "twOrbitControl.h"
#include "twUtil.h"
#include "twScene.h"

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

    virtual void UpdateViews() override;

private:
    AntiAliasingView* _view;
    AntiAliasingView* _view2;
};

class AntiAliasingScene
{
public:
    AntiAliasingScene()
    {}
    ~AntiAliasingScene()
    {}

private:
    Scene::Ptr _scene;
};

class AntiAliasingView : public View
{
public:
    AntiAliasingView()
        : View()
        , _current_aa_option(-1)
        {}

    virtual ~AntiAliasingView()
    {}

protected:
    virtual void Initialize() override;
    virtual void Destroy() override;
    virtual void Advance(float64 delta_time) override;
    virtual void RenderImpl() override;
    virtual void OnGUI() override;

private:
    void CreateScene();
    void UpdateScene();
    void RenderScene();
    void DestroyScene();

    void CreateGeometry(Geometry::Ptr geom, uint32 index);
    void RenderGeometry(Geometry::Ptr geom, int32 index);

private:
    enum AAOption
    {
        MSAA_HW = 0,
        MSAA_SW = 1,
        SSAA = 2,
        CSAA = 3,
        CFAA = 4,
        FXAA = 5
    };

    glm::mat4 _view_mat;
    glm::mat4 _projection_mat;    
    glm::mat4 _mvp_mat;
    glm::mat4 _origin_mvp_mat;

    glm::vec4 _viewport_params;

    Plane::Ptr _plane_left;
    Plane::Ptr _plane_top;
    Plane::Ptr _plane_right;
    Plane::Ptr _plane_bottom;
    Plane::Ptr _plane_back;

    IcosahedronSphere::Ptr _sphere;
    Cube::Ptr _cube;
    Triangle::Ptr _triangle_back;
    Triangle::Ptr _triangle_front;

    Transform::Ptr _root_trans;

    uint32 *_vaos;
    uint32 *_vbos;
    uint32 *_ebos;

    int32 _current_aa_option;

    friend class AntiAliasing;
};

}

#endif