

#include "twSprite.h"
#include "twMaterialInstance.h"

namespace TwinkleGraphics
{
Sprite::Sprite(Texture::Ptr texture)
    : Quad()
    , _perpixelunit(100)
{
    Init(texture);
}

Sprite::Sprite(Texture::Ptr texture, glm::vec2 size)
    : Quad()
    , _perpixelunit(100)
{
    Init(texture, size);
}

Sprite::~Sprite()
{
}

void Sprite::SetFlip(bvec2 flip)
{
    if(_sprite_renderer != nullptr)
    {
        _sprite_renderer->SetFlip(flip);
    }
}

void Sprite::SetColor(vec4 &color)
{
    if(_sprite_renderer != nullptr)
    {
        _sprite_renderer->SetColor(color);
    }
}

/**
 * @brief when texture changed, mesh & material settting should follow
 *
 * @param texture
 */
void Sprite::SetTexture(Texture::Ptr texture)
{
    if(_sprite_renderer != nullptr)
    {

    }
}


void Sprite::Init(Texture::Ptr texture)
{
#ifdef _DEBUG
    assert(texture != nullptr);
#endif

    // set sprite renderer
    _sprite_renderer = std::make_shared<SpriteRenderer>(texture);
    SetMeshRenderer(_sprite_renderer);

    _flag = MeshDataFlag(9);
    glm::vec2 size = glm::vec2(texture->GetWidth(0), texture->GetHeight(0)) / (float32)_perpixelunit * 2.0f;
    Quad::SetSize(size);
    Quad::GenerateMesh();

    _sprite_renderer->SetMesh(_mesh);
}

void Sprite::Init(Texture::Ptr texture, glm::vec2 size)
{
#ifdef _DEBUG
    assert(texture != nullptr);
#endif

    // set sprite renderer
    _sprite_renderer = std::make_shared<SpriteRenderer>(texture);
    SetMeshRenderer(_sprite_renderer);

    _flag = MeshDataFlag(9);
    Quad::SetSize(size);
    Quad::GenerateMesh();

    _sprite_renderer->SetMesh(_mesh);
}

/*-------------------------------Sprite Renderer-------------------------------*/


SpriteRenderer::SpriteRenderer(Texture::Ptr texture)
    : MeshRenderer()
{
    Init(texture);
}

SpriteRenderer::~SpriteRenderer()
{
}


void SpriteRenderer::Init(Texture::Ptr texture)
{
    if(texture == nullptr) return;

    const RenderResInstance& res = texture->GetRenderRes();
    ShaderManagerInst shaderMgr;
    ShaderProgram::Ptr program = nullptr;
    if(res.type == (int)(TextureType::TEXTURE_1D))
    {
        Sprite1DMaterial::Ptr mat = std::make_shared<Sprite1DMaterial>();
        mat->SetMainTexture(texture);
        SetMaterial(mat);
    }
    else if(res.type == (int)(TextureType::TEXTURE_2D))
    {
        SpriteMaterial::Ptr mat = std::make_shared<SpriteMaterial>();
        mat->SetMainTexture(texture);
        SetMaterial(mat);
    }
}

void SpriteRenderer::SetFlip(bvec2 flip)
{
    if(_shared_material != nullptr)
    {
        _shared_material->SetVecUniformValue<bool, 2>("flip", flip);
    }
}

void SpriteRenderer::SetColor(vec4 &color)
{
    if(_shared_material != nullptr)
    {
        _shared_material->SetVecUniformValue<float32, 4>("tint_color", color);
    }
}

} // namespace TwinkleGraphics
