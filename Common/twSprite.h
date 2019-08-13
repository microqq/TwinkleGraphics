
#ifndef TW_SPRITE_H
#define TW_SPRITE_H

#include "twMesh.h"
#include "twTexture.h"

namespace TwinkleGraphics
{
class Sprite : public Quad
{
public:
    typedef std::shared_ptr<Sprite> Ptr;

    Sprite(Texture2D::Ptr texture);
    virtual ~Sprite();

    void SetTexture(Texture2D::Ptr texture) { if(_texture != texture) { _texture = texture; _texdirty = true; } }

private:
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
        }
    }

    void UpdateSize()
    {}

private:
    Texture2D::Ptr _texture;
    int32 _perpixelunit;

    bool _texdirty;
};


} // namespace TwinkleGraphics

#endif