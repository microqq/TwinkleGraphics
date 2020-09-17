
#ifndef TW_SPRITE_H
#define TW_SPRITE_H

#include "twGeometry.h"
#include "twTexture.h"

namespace TwinkleGraphics
{
class __TWCOMExport SpriteRenderer : public MeshRenderer
{
public:
    typedef std::shared_ptr<SpriteRenderer> Ptr;

    SpriteRenderer(Texture::Ptr texture);
    virtual ~SpriteRenderer();

    void SetFlip(bvec2 flip);
    void SetColor(vec4& color);

private:
    void Init(Texture::Ptr texture);

private:
};


class __TWCOMExport Sprite : public Quad
{
public:
    typedef std::shared_ptr<Sprite> Ptr;

    Sprite(Texture::Ptr texture, MeshDataFlag flag = MeshDataFlag(8));
    Sprite(Texture::Ptr texture, glm::vec2 size, MeshDataFlag flag = MeshDataFlag(8));
    virtual ~Sprite();

    void SetFlip(bvec2 flip);
    void SetColor(vec4& color);
    void SetTexture(Texture::Ptr texture);
    Texture::Ptr GetTexture()
    {
        if (_renderer == nullptr)
        {
            return nullptr;
        }

        Material::Ptr mat = _renderer->GetMaterial();
        if(mat == nullptr)
        {
            return nullptr;
        }
        return mat->GetMainTexture();
    }
    Material::Ptr GetMaterial() { return _renderer->GetMaterial(); }

private:
    void InitRenderer(Texture::Ptr texture);
    void Resize() {}

private:
    SpriteRenderer::Ptr _spriteRenderer;
    int32 _perpixelunit = 100;
};


} // namespace TwinkleGraphics

#endif