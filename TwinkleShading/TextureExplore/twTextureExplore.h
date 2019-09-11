


#ifndef TW_TEXTUREEXPLORE_H
#define TW_TEXTUREEXPLORE_H

#include "twCommon.h"

#include "twPlugin.h"
#include "twShader.h"
#include "twMesh.h"
#include "twCamera.h"
#include "twOrbitControl.h"
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
    CameraControl* _camera_control;
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
        , _wrap_modes()
        , _wrap_option(-1)
        , _filter_modes()
        , _filter_option(-1)
        , _swizzle_option(-1)
        , _current_tex_option(-1)
        , _lodbias_value(1.0f)
        , _swizzle_masks()
        , _texparams_tabitem()
        , _enable_lodbias(false)
        {
            _wrap_modes[0] = _wrap_modes[1] = _wrap_modes[2] = -1;
            _filter_modes[0] = _filter_modes[1] = -1;
            _swizzle_masks[0] = 0;
            _swizzle_masks[1] = 1;
            _swizzle_masks[2] = 2;
            _swizzle_masks[3] = 3;

            _tintColor[0] = _tintColor[1] = _tintColor[2] = _tintColor[3] = 1.0f;

            _tex_tiling = vec2(1.0f, 1.0f);
            _tex_offset = vec2 (0.0f, 0.0f);
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
    void RenderSprite();
    
    void CreateGeometry(Geometry::Ptr geom, uint32 index);
    void RenderGeometry(Geometry::Ptr geom, int32 index, GLenum front_face = GL_CCW);

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
    glm::mat4 _model_mat;
    glm::mat4 _view_mat;
    glm::mat4 _projection_mat;    
    glm::mat4 _mvp_mat;

    uint32 *_vaos;
    uint32 *_vbos;
    uint32 *_ebos;

    TexParams _texparams;

    Texture1D::Ptr _texture1D;
    Texture2D::Ptr _texture2D;
    Texture3D::Ptr _texture3D;
    TextureCube::Ptr _textureCube;
    Texture1DArray::Ptr _texture1DArray;
    Sprite::Ptr _sprite;

    glm::vec4 _viewport_params;
    float32 _tintColor[4];
    glm::vec2 _tex_tiling;
    glm::vec2 _tex_offset;

    int32 _current_tex_option;
    int32 _wrap_modes[3];
    int32 _wrap_option;
    int32 _filter_modes[2];
    int32 _filter_option;
    int32 _swizzle_option;
    int32 _swizzle_masks[4];

    float32 _lodbias_value;
    bool _texparams_tabitem[4];
    bool _enable_lodbias;

    friend class TextureExplore;
};

}

#endif