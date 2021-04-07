


#ifndef TW_TEXTUREEXPLORE_H
#define TW_TEXTUREEXPLORE_H

#include "twCommon.h"

#include "twPlugin.h"
#include "twShader.h"
#include "twMesh.h"
#include "twCamera.h"
#include "twFirstPersonControl.h"
#include "twTexture.h"
#include "twSprite.h"


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
};

class TextureExploreView : public View
{
public:
    TextureExploreView()
        : View()
        , _vaos(nullptr)
        , _vbos(nullptr)
        , _ebos(nullptr)
        , _sprite(nullptr)
        , _wrapModes()
        , _wrapOption(-1)
        , _filterModes()
        , _filterOption(-1)
        , _swizzleOption(-1)
        , _currentTexOption(-1)
        , _lodbiasValue(1.0f)
        , _updateTime(0.0f)
        , _swizzleMasks()
        , _texparamsTabitem()
        , _enableLodBias(false)
        , _enableBorderColor(false)
        , _enableSkybox(false)
        {
            _wrapModes[0] = _wrapModes[1] = _wrapModes[2] = -1;
            _filterModes[0] = _filterModes[1] = -1;
            _swizzleMasks[0] = 0;
            _swizzleMasks[1] = 1;
            _swizzleMasks[2] = 2;
            _swizzleMasks[3] = 3;

            _tintcolor[0] = _tintcolor[1] = _tintcolor[2] = _tintcolor[3] = 1.0f;
            _bordercolor = vec4(0.0f, 0.0f, 0.0f, 0.0f);

            _texTiling = vec2(1.0f, 1.0f);
            _texOffset = vec2 (0.0f, 0.0f);
        }

    virtual ~TextureExploreView()
    {}

protected:
    virtual void Initialize() override;
    virtual void Destroy() override;
    virtual void Advance(float64 delta_time) override;
    virtual void RenderImpl() override;
    virtual void OnGUI() override;

private:
    void CreateSprite();
    void CreateSprite1D();
    void CreateVolumnTexture();
    void CreateSkybox();
    void CreateCube(ImagePtr image);
    void CreateIconSphere(ImagePtr image);
    void CreateNURBSSurface();
    
    void CreateGeometry(GeometryPtr geom, uint32 index);

    void RenderGeometryEx(int index);
    void RenderGeometry(GeometryPtr geom, int32 index, GLenum front_face = GL_CCW);

    void OnWrapModeGUI(int32& wrap_mode_option);
    void OnFilterModeGUI(int32& filter_mode_option);
    void OnSwizzleModeGUI();
    void ResetGUI();

    void SetTexparams();
    WrapMode GetWrapMode(int32 wrap_mode_option);
    FilterMode GetFilterMode(int32 filter_mode_option);
    SwizzleParam GetSwizzleParam(int32 swizzle_option);
    SwizzleMask GetSwizzleMask(int32 swizzle_mask_option);


private:
    glm::mat4 _modelMat;
    glm::mat4 _viewMat;
    glm::mat4 _projectionMat;    
    glm::mat4 _mvpMat;

    uint32 *_vaos;
    uint32 *_vbos;
    uint32 *_ebos;

    TexParams _texparams;

    SpritePtr _sprite;
    SpritePtr _sprite1D;

    QuadPtr _volumnQuad;
    CubePtr _skybox;
    CubePtr _cube;
    IcosahedronSpherePtr _sphere;
    NURBSSurfacePtr _nurbsSurface;
    QuadPtr _projTexQuad;

    CameraPtr _projTexCamera;

    glm::vec4 _viewportParams;
    float32 _tintcolor[4];
    glm::vec4 _bordercolor;
    glm::vec2 _texTiling;
    glm::vec2 _texOffset;

    int32 _currentTexOption;
    int32 _wrapModes[3];
    int32 _wrapOption;
    int32 _filterModes[2];
    int32 _filterOption;
    int32 _swizzleOption;
    int32 _swizzleMasks[4];

    float32 _lodbiasValue;

    float32 _updateTime;

    bool _texparamsTabitem[4];
    bool _enableLodBias;
    bool _enableBorderColor;
    bool _enableSkybox;

    friend class TextureExplore;
};

}

#endif