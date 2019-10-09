
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

    SpriteRenderer(int type, Texture::Ptr texture);
    virtual ~SpriteRenderer();

    void SetFlip(bool flip);
    void SetColor(vec4& color);

private:
    void Init(int type, Texture::Ptr texture);

private:
};


class Sprite : public Quad
{
public:
    typedef std::shared_ptr<Sprite> Ptr;

    Sprite(Texture::Ptr texture);
    Sprite(int type, Texture::Ptr texture, glm::vec2 size);
    virtual ~Sprite();

    void SetFlip(bool flip);
    void SetColor(vec4& color);
    void SetTexture(Texture::Ptr texture);
    Texture::Ptr GetTexture() { if(_renderer == nullptr || _renderer->GetSharedMaterial() == nullptr) return nullptr; return _renderer->GetSharedMaterial()->GetMainTexture(); }
    Material::Ptr GetMaterial() { return _renderer->GetSharedMaterial(); }

private:
    void Init(Texture::Ptr texture);
    void Init(int type, Texture::Ptr texture, glm::vec2 size);

    void GenerateUVs()
    {
        if(_mesh != nullptr)
        {
            SubMesh::Ptr submesh = _mesh->GetSubMesh(0);
            glm::vec4* uvs = submesh->GetVerticeUV();

            /**
             * @brief
             * 0 __ __ __ 3
             *  |        |
             *  |        |
             *  |__ __ __|
             * 1          2
             */
            
            uvs[0] = glm::vec4(0.0f, 1.0f, 0.0, 0.0f);
            uvs[1] = glm::vec4(0.0f, 0.0f, 0.0, 0.0f);
            uvs[2] = glm::vec4(1.0f, 0.0f, 0.0, 0.0f);
            uvs[3] = glm::vec4(1.0f, 1.0f, 0.0, 0.0f);

            _renderer->SetMesh(_mesh);
        }
    }

    void UpdateSize()
    {}

private:
    SpriteRenderer::Ptr _sprite_renderer;
    int32 _perpixelunit;
};


} // namespace TwinkleGraphics

#endif