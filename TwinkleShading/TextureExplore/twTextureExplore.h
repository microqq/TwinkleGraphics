


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
    void CreateSprite();
    void RenderSprite();
    
    void CreateGeometry(Geometry::Ptr geom, uint32 index);
    void RenderGeometry(Geometry::Ptr geom, int32 index, GLenum front_face = GL_CCW);


private:
    glm::mat4 _model_mat;
    glm::mat4 _view_mat;
    glm::mat4 _projection_mat;    
    glm::mat4 _mvp_mat;

    uint32 *_vaos;
    uint32 *_vbos;
    uint32 *_ebos;

    glm::vec4 _viewport_params;

    Texture1D::Ptr _texture1D;
    Texture2D::Ptr _texture2D;
    Texture3D::Ptr _texture3D;
    TextureCube::Ptr _textureCube;
    Texture1DArray::Ptr _texture1DArray;

    Sprite::Ptr _sprite;

    int32 _current_tex_option;

    friend class TextureExplore;
};

}

#endif