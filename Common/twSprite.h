
#ifndef TW_SPRITE_H
#define TW_SPRITE_H

#include "twGeometry.h"
#include "twTexture.h"

namespace TwinkleGraphics
{
class SpriteRenderer : public MeshRenderer
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


class Sprite : public Quad
{
public:
    typedef std::shared_ptr<Sprite> Ptr;

    Sprite(Texture::Ptr texture);
    Sprite(Texture::Ptr texture, glm::vec2 size);
    virtual ~Sprite();

    void SetFlip(bvec2 flip);
    void SetColor(vec4& color);
    void SetTexture(Texture::Ptr texture);
    Texture::Ptr GetTexture() { if(_renderer == nullptr || _renderer->GetSharedMaterial() == nullptr) return nullptr; return _renderer->GetSharedMaterial()->GetMainTexture(); }
    Material::Ptr GetMaterial() { return _renderer->GetSharedMaterial(); }

private:
    void Init(Texture::Ptr texture);
    void Init(Texture::Ptr texture, glm::vec2 size);

    void UpdateSize()
    {}

private:
    SpriteRenderer::Ptr _sprite_renderer;
    int32 _perpixelunit;
};


} // namespace TwinkleGraphics

#endif