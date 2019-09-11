

#include "twSprite.h"

namespace TwinkleGraphics
{
Sprite::Sprite(Texture2D::Ptr texture)
    : Quad()
    , _perpixelunit(100)
{
    Init(texture);
}

Sprite::~Sprite()
{
}

void Sprite::SetFlip(bool flip)
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
void Sprite::SetTexture(Texture2D::Ptr texture)
{
    if(_sprite_renderer != nullptr)
    {

    }
}


void Sprite::Init(Texture2D::Ptr texture)
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
    GenerateUVs();
}



/*-------------------------------Sprite Renderer-------------------------------*/


SpriteRenderer::SpriteRenderer(Texture2D::Ptr texture)
    : MeshRenderer()
{
    Init(texture);
}

SpriteRenderer::~SpriteRenderer()
{
}


void SpriteRenderer::Init(Texture2D::Ptr texture)
{
    ShaderManagerInst shaderMgr;
    ShaderReadInfo sprite_shader_info[] = {
        {std::string("Assets/Shaders/sprite.vert"), ShaderType::VERTEX_SHADER},
        {std::string("Assets/Shaders/sprite.frag"), ShaderType::FRAGMENT_SHADER}};
    ShaderProgram::Ptr sprite_shader = shaderMgr->ReadShaders(sprite_shader_info, 2);

    Material::Ptr sprite_mat = std::make_shared<Material>();
    RenderPass::Ptr pass = std::make_shared<RenderPass>(sprite_shader);
    sprite_mat->AddRenderPass(pass);

    vec4 tint_color(1.0f, 1.0f, 1.0f, 1.0f);
    sprite_mat->SetSimpleUniformValue<bool, 1>("flip", false);
    sprite_mat->SetVecUniformValue<float32, 4>("tint_color", tint_color);
    vec2 tiling(1.0f, 1.0f), offset(0.0f, 0.0f);
    sprite_mat->SetTextureTiling("main_tex", tiling);
    sprite_mat->SetTextureOffset("main_tex", offset);

    sprite_mat->SetMainTexture(texture);

    SetMaterial(sprite_mat);
}

void SpriteRenderer::SetFlip(bool flip)
{
    if(_shared_material != nullptr)
    {
        _shared_material->SetSimpleUniformValue<bool, 1>("flip", flip);
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
