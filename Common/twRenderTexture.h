#ifndef TW_RENDERTEXTURE_H
#define TW_RENDERTEXTURE_H

#include "twTexture.h"

namespace TwinkleGraphics
{
class RenderTexture : public Object
{
public:
    typedef std::shared_ptr<RenderTexture> Ptr;

    RenderTexture(int32 width, int32 height, GLenum internalformat, GLenum format
        , bool antialiasing = false, int32 samples = 1, bool hasdepthstencil = false, int32 depth = 1, bool fixedsampledlocation = true);
    virtual ~RenderTexture();

private:
    void InitStorage(int32 width, int32 height, GLenum internalformat, GLenum format
        , bool antialiasing, int32 samples, bool hasdepthstencil, int32 depth, bool fixedsampledlocation);

private:
    Texture::Ptr _texture;
};

} // namespace TwinkleGraphics



#endif