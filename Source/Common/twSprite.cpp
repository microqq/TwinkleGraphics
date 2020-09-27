

#include "twSprite.h"
#include "twMaterialInstance.h"

namespace TwinkleGraphics
{
Sprite::Sprite(Texture::Ptr texture, MeshDataFlag flag)
    : Quad(flag)
    , _perpixelunit(100)
{
    glm::vec2 size = glm::vec2(texture->GetWidth(0), texture->GetHeight(0)) / (float32)_perpixelunit * 2.0f;
    Quad::SetSize(size);
    Quad::GenerateMeshInternal(flag);

    InitRenderer(texture);
}

Sprite::Sprite(Texture::Ptr texture, glm::vec2 size, MeshDataFlag flag)
    : Quad(size
        , flag)
    , _perpixelunit(100)
{
    InitRenderer(texture);
}

Sprite::~Sprite()
{
}

void Sprite::SetFlip(bvec2 flip)
{
    if(_spriteRenderer != nullptr)
    {
        _spriteRenderer->SetFlip(flip);
    }
}

void Sprite::SetColor(vec4 &color)
{
    if(_spriteRenderer != nullptr)
    {
        _spriteRenderer->SetColor(color);
    }
}

/**
 * @brief when texture changed, mesh & material settting should follow
 *
 * @param texture
 */
void Sprite::SetTexture(Texture::Ptr texture)
{
    if(_spriteRenderer != nullptr)
    {

    }
}


void Sprite::InitRenderer(Texture::Ptr texture)
{
#ifdef _DEBUG
#endif

    // set sprite renderer
    _spriteRenderer = std::make_shared<SpriteRenderer>(texture);
    SetMeshRenderer(_spriteRenderer);
    _spriteRenderer->SetMesh(_mesh);
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

    const RenderResourceHandle& res = texture->GetRenderRes();
    ShaderProgram::Ptr program = nullptr;
    if(res.type == (int)(TextureType::TEXTURE_1D))
    {
        Material::Ptr mat = std::make_shared<Sprite1DMaterial>();
        SetMaterial(mat);
        mat = GetMaterial(); 
        mat->SetMainTexture(texture);
    }
    else if(res.type == (int)(TextureType::TEXTURE_2D))
    {
        Material::Ptr mat = std::make_shared<SpriteMaterial>();
        SetMaterial(mat);
        mat = GetMaterial(); 
        GetMaterial()->SetMainTexture(texture);
    }
}

void SpriteRenderer::SetFlip(bvec2 flip)
{
    GetMaterial()->SetVecUniformValue<bool, 2>("flip", flip);
}

void SpriteRenderer::SetColor(vec4 &color)
{
    GetMaterial()->SetVecUniformValue<float32, 4>("tintColor", color);
}

} // namespace TwinkleGraphics
