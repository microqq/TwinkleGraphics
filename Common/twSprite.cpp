

#include "twSprite.h"

namespace TwinkleGraphics
{
Sprite::Sprite(Texture::Ptr texture)
    : Quad()
    , _perpixelunit(100)
{
    Init(texture);
}

Sprite::Sprite(int type, Texture::Ptr texture, glm::vec2 size)
    : Quad()
    , _perpixelunit(100)
{
    Init(type, texture, size);
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
    _sprite_renderer = std::make_shared<SpriteRenderer>(GL_TEXTURE_2D, texture);
    SetMeshRenderer(_sprite_renderer);

    _flag = MeshDataFlag(9);
    glm::vec2 size = glm::vec2(texture->GetWidth(0), texture->GetHeight(0)) / (float32)_perpixelunit * 2.0f;
    Quad::SetSize(size);
    Quad::GenerateMesh();
    GenerateUVs();
}

void Sprite::Init(int type, Texture::Ptr texture, glm::vec2 size)
{
#ifdef _DEBUG
    assert(texture != nullptr);
#endif

    // set sprite renderer
    _sprite_renderer = std::make_shared<SpriteRenderer>(GL_TEXTURE_1D, texture);
    SetMeshRenderer(_sprite_renderer);

    _flag = MeshDataFlag(9);
    Quad::SetSize(size);
    Quad::GenerateMesh();
    GenerateUVs();
}

/*-------------------------------Sprite Renderer-------------------------------*/


SpriteRenderer::SpriteRenderer(int type, Texture::Ptr texture)
    : MeshRenderer()
{
    Init(type, texture);
}

SpriteRenderer::~SpriteRenderer()
{
}


void SpriteRenderer::Init(int type, Texture::Ptr texture)
{
    ShaderManagerInst shaderMgr;
    ShaderProgram::Ptr program = nullptr;
    if(type == GL_TEXTURE_1D)
    {
        ShaderReadInfo sprite_shader_info[] = {
            {std::string("Assets/Shaders/sprite.vert"), ShaderType::VERTEX_SHADER},
            {std::string("Assets/Shaders/sprite_1d.frag"), ShaderType::FRAGMENT_SHADER}};
        program = shaderMgr->ReadShaders(sprite_shader_info, 2);
    }
    else if(type == GL_TEXTURE_2D)
    {
        ShaderReadInfo sprite_shader_info[] = {
            {std::string("Assets/Shaders/sprite.vert"), ShaderType::VERTEX_SHADER},
            {std::string("Assets/Shaders/sprite.frag"), ShaderType::FRAGMENT_SHADER}};
        program = shaderMgr->ReadShaders(sprite_shader_info, 2);
    }


    Material::Ptr sprite_mat = std::make_shared<Material>();
    RenderPass::Ptr pass = std::make_shared<RenderPass>(program);
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
