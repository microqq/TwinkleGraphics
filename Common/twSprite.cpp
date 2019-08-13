

#include "twSprite.h"

namespace TwinkleGraphics
{
Sprite::Sprite(Texture2D::Ptr texture)
    : Quad(MeshDataFlag(9))
    , _texture(texture)
    , _perpixelunit(100)
{
#ifdef _DEBUG
    assert(texture != nullptr);
#endif

    glm::vec2 size = glm::vec2(texture->GetWidth(0), texture->GetHeight(0)) / (float32)_perpixelunit * 2.0f;
    Quad::SetSize(size);
    Quad::Generate();
    GenerateUVs();
}

Sprite::~Sprite()
{
}

} // namespace TwinkleGraphics
