


#ifndef TW_TEXTUREEXPLORE_H
#define TW_TEXTUREEXPLORE_H

#include "twCommon.h"

#include "twPlugin.h"
#include "twShader.h"
#include "twMesh.h"
#include "twCamera.h"
#include "twOrbitControl.h"
#include "twUtil.h"
#include "twSceneManagement.h"
#include "twRenderTexture.h"

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
    // AntiAliasingView* _view2;
};

class AntiAliasingScene : public Scene
{
public:
    typedef std::shared_ptr<AntiAliasingScene> Ptr;

    AntiAliasingScene()
        : Scene()
        , _current_aa_option(-1)
    {}
    ~AntiAliasingScene()
    {}

    virtual void Init() override
    {
        //create vertex buffer object
        _vbos = new uint32[16];
        glGenBuffers(16, _vbos);
        _ebos = new uint32[16];
        glGenBuffers(16, _ebos);

        //create vertex array object
        _vaos = new uint32[16];
        glGenVertexArrays(16, _vaos);

        Viewport viewport(Rect(0, 0, 1024, 768), 17664U, RGBA(0.0f, 0.f, 0.0f, 1.f));
        _msaa_camera = std::make_shared<Camera>(viewport, 45.0f, 0.1f, 1000.0f);
        _msaa_camera->Translate(glm::vec3(0.0f, 0.0f, 25));

        CreateScene();
    }

    virtual void Update(float32 delta_time) override
    {
        UpdateScene();
    }

    virtual void Render() override
    {
        Scene::Render();
        RenderScene();
    }

    int32& GetCurrentAAOption() { return _current_aa_option; }

private:
    void CreateScene();
    void UpdateScene();
    void RenderScene();
    void DestroyScene();

    void CreateGeometry(Geometry::Ptr geom, uint32 index);
    void RenderGeometry(Geometry::Ptr geom, int32 index);

    void RenderScreenQuad();

    void RenderGeometrys();

private:
    enum AAOption
    {
        NONE = -1,
        MSAA_HW = 0,
        MSAA_SW = 1,
        SSAA = 2,
        CSAA = 3,
        CFAA = 4,
        FXAA = 5
    };

    enum MSAASWOption
    {
        MSAA_SW_BACKBUFFER = 1,
        MSAA_SW_SCREENFBO = 2,
        MSAA_SW_RESOLVE = 3
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
    Camera::Ptr _msaa_camera;

    // msaa resovle filter
    RenderTexture::Ptr _rt_msaa;
    RenderTexture::Ptr _rt_msaa_resolve;
    Quad::Ptr _screen_quad_msaa;
    Material::Ptr _screen_quad_mat;
    Material::Ptr _msaa_resolve_mat;

    uint32 *_vaos;
    uint32 *_vbos;
    uint32 *_ebos;

    int32 _current_aa_option;
    int32 _current_msaa_sw_option = 1;

    bool _enable_multisample = true;

    friend class AntiAliasingView;
};

class AntiAliasingView : public View
{
public:
    using AAOption = AntiAliasingScene::AAOption;
    using MSAASWOption = AntiAliasingScene::MSAASWOption;

    AntiAliasingView()
        : View()
        {}

    virtual ~AntiAliasingView()
    {}

protected:
    virtual void Initialize() override;
    virtual void Destroy() override;
    virtual void OnGUI() override;

private:
    friend class AntiAliasing;
};

}

#endif