
#ifndef TW_SPRITE_H
#define TW_SPRITE_H

#include "twMesh.h"
#include "twTexture.h"

namespace TwinkleGraphics
{
class Sprite : public Quad
{
public:
    Sprite();
    virtual ~Sprite();

    void SetTexture(Texture2D::Ptr texture) { _texture = texture; }
    virtual void Generate() override
    {
        Quad::Generate();

        if(_mesh != nullptr)
        {
        }
    }

private:
    Texture2D::Ptr _texture;
};


} // namespace TwinkleGraphics

#endif