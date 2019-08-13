

#include "twSprite.h"

namespace TwinkleGraphics
{
Sprite::Sprite()
    : Quad(glm::vec2(1.0f, 1.0f), MeshDataFlag(9))
{
}

Sprite::~Sprite()
{
}

} // namespace TwinkleGraphics
