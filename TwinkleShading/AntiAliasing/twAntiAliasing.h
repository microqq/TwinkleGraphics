


#ifndef TW_TEXTUREEXPLORE_H
#define TW_TEXTUREEXPLORE_H

#include "twCommon.h"

#include "twPlugin.h"
#include "twShader.h"
#include "twMesh.h"
#include "twCamera.h"
#include "twFirstPersonControl.h"
#include "twUtil.h"
#include "twSceneManager.h"
#include "twRenderTexture.h"
#include "twModelReader.h"
#include "twUiUtil.h"

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
        , _currentAAOption(-1)
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
        _msaaCamera = std::make_shared<Camera>(viewport, 45.0f, 0.1f, 1000.0f);
        _msaaCamera->Translate(glm::vec3(0.0f, 0.0f, 25));

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

    int32& GetCurrentAAOption() { return _currentAAOption; }
    void Load3DModel(std::string filename);

private:
    void SetSamplesNum(int samples) 
    {
         _numSamples = samples;

         //reset _rt_msaa
    }

    void CreateScene();
    void UpdateScene();
    void RenderScene();
    void DestroyScene();

    void CreateGeometry(Geometry::Ptr geom, uint32 index);
    void CreateSkybox();
    void CreateInfinitePlane();

    void RenderGeometry(Geometry::Ptr geom, int32 index);
    void RenderGeometrys();
    void RenderScreenQuad();
    void RenderSkybox();
    void RenderInfinitePlane();

    void ResizeScreenTexture();

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

    enum ResolveFilterOption
    {
        BOX = 0,
        TRIANGLE = 1,
        CUBIC = 2,
        HANN_HAMMINGWINDOW = 3,
        BLACKMANWINDOW = 4,
        KAISERWINDOW = 5,
        LANCZOS3WINDOW = 6,
        LANCZOS4WINDOW = 7,
        LANCZOS6WINDOW = 8
    };

    glm::mat4 _viewMat;
    glm::mat4 _projectionMat;    
    glm::mat4 _mvpMat;
    glm::mat4 _originMVPMat;

    glm::vec4 _viewportParams;

    Plane::Ptr _planeLeft;
    Plane::Ptr _planeTop;
    Plane::Ptr _planeRight;
    Plane::Ptr _planeBottom;
    Plane::Ptr _planeBack;

    IcosahedronSphere::Ptr _sphere;
    Cube::Ptr _cube;
    Triangle::Ptr _triangleBack;
    Triangle::Ptr _triangle_front;

    Cube::Ptr _skybox;
    Plane::Ptr _infinitePlane;

    SceneNode::Ptr _rootNode;
    Transform::Ptr _rootTrans;
    Camera::Ptr _msaaCamera;

    // msaa resovle filter
    RenderTexture::Ptr _rtMSAA;
    RenderTexture::Ptr _rtScreen;
    Quad::Ptr _screenQuadMSAA;
    Material::Ptr _screenQuadMat;
    Material::Ptr _msaaResolveMat;

    Model::Ptr _model = nullptr;

    uint32 *_vaos;
    uint32 *_vbos;
    uint32 *_ebos;

    int32 _currentAAOption;
    int32 _currentMSAASWOption = 1;
    int32 _resolveFitlerOption = 0;
    int32 _numSamples = 4;

    bool _enableMultisample = true;

    friend class AntiAliasingView;
};

class AntiAliasingView : public View
{
public:
    using AAOption = AntiAliasingScene::AAOption;
    using MSAASWOption = AntiAliasingScene::MSAASWOption;
    using ResolveFilterOption = AntiAliasingScene::ResolveFilterOption;

    AntiAliasingView()
        : View()
        {}

    virtual ~AntiAliasingView()
    {}

protected:
    virtual void Initialize() override;
    virtual void Destroy() override;
    virtual void OnGUI() override;

    void Select3DModel();

private:
    FileDialogSelectInfo _selectFileInfo;

    friend class AntiAliasing;
};

}

#endif