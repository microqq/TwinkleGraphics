
#ifndef TW_SPRITE_H
#define TW_SPRITE_H

#include "twGeometry.h"
#include "twTexture.h"

namespace TwinkleGraphics {
class __TWCOMExport SpriteRenderer : public MeshRenderer {
public:
  typedef std::shared_ptr<SpriteRenderer> Ptr;

  SpriteRenderer(TexturePtr texture);
  virtual ~SpriteRenderer();

  void SetFlip(bvec2 flip);
  void SetColor(vec4 &color);

private:
  void Init(TexturePtr texture);

private:
};

typedef SpriteRenderer::Ptr SpriteRendererPtr;

class __TWCOMExport Sprite : public Quad {
public:
  typedef std::shared_ptr<Sprite> Ptr;

  Sprite(TexturePtr texture, MeshDataFlag flag = MeshDataFlag(8));
  Sprite(TexturePtr texture, glm::vec2 size,
         MeshDataFlag flag = MeshDataFlag(8));
  virtual ~Sprite();

  void SetFlip(bvec2 flip);
  void SetColor(vec4 &color);
  void SetTexture(TexturePtr texture);
  TexturePtr GetTexture() {
    if (_renderer == nullptr) {
      return nullptr;
    }

    MaterialPtr mat = _renderer->GetMaterial();
    if (mat == nullptr) {
      return nullptr;
    }
    return mat->GetMainTexture();
  }
  MaterialPtr GetMaterial() { return _renderer->GetMaterial(); }

private:
  void InitRenderer(TexturePtr texture);
  void Resize() {}

private:
  SpriteRendererPtr _spriteRenderer;
  int32 _perpixelunit = 100;
};

typedef Sprite::Ptr SpritePtr;

} // namespace TwinkleGraphics

#endif