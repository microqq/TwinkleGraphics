#ifndef TW_RENDERTEXTURE_H
#define TW_RENDERTEXTURE_H

#include "twTexture.h"

namespace TwinkleGraphics
{
class RenderTexture2D : public Texture2D
{
public:
    typedef std::shared_ptr<RenderTexture2D> Ptr;

    RenderTexture2D(int32 width, int32 height);
    ~RenderTexture2D();

protected:
    virtual void InitStorage() override; 

private:
    int32 _width, _height;

};

class RenderTexture2DMS : public Texture2DMultiSample
{
public:
    RenderTexture2DMS(int32 width, int32 height);
    ~RenderTexture2DMS();

protected:
    virtual void InitStorage() override; 

private:
    int32 _width, _height;
};

} // namespace TwinkleGraphics



#endif