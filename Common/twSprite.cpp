

#include "twSprite.h"

namespace TwinkleGraphics
{
Sprite::Sprite(Texture2D::Ptr texture)
    : Quad()
    , _perpixelunit(100)
{
#ifdef _DEBUG
    assert(texture != nullptr);
#endif

    MeshRenderer::Ptr renderer(new SpriteRenderer());
    SetMeshRenderer(renderer);

    _flag = MeshDataFlag(9);

    glm::vec2 size = glm::vec2(texture->GetWidth(0), texture->GetHeight(0)) / (float32)_perpixelunit * 2.0f;
    Quad::SetSize(size);
    Quad::GenerateMesh();
    GenerateUVs();
}

Sprite::~Sprite()
{
}



/*-------------------------------Sprite Renderer-------------------------------*/


SpriteRenderer::SpriteRenderer()
    : MeshRenderer()
    , _tintcolor(vec4(1.0f, 1.0f, 1.0f, 1.0f))
    , _flip(false)
{
}

SpriteRenderer::~SpriteRenderer()
{
}

} // namespace TwinkleGraphics
