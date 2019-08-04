

#ifndef TW_TEXTURE_H
#define TW_TEXTURE_H

#include "twCommon.h"
#include <glew/glew.h>


namespace TwinkleGraphics
{
class TextureManager
{
public:
    TextureManager();
    ~TextureManager();

private:

};

enum class WrapMode
{

};

enum class SamplerMode
{
    
};

class Texture : public Object
{
public:
    typedef std::shared_ptr<Texture> Ptr;

    Texture();
    virtual ~Texture();

    void SetHeight(int32 height);
    void SetWidth(int32 width);

protected:
    RenderResInstance _res;

    int32 _height, _width;
    int32 _num_mipmap_levels;
};

class Texture1D : public Texture
{
public:
    typedef std::shared_ptr<Texture1D> Ptr;

    Texture1D();
    virtual ~Texture1D() = 0;
};

class Texture2D : public Texture
{};

class Texture2DMultiSampler : public Texture
{

};

class Texture3D : public Texture
{};



class TextureRectangle : public Texture
{

};

class TextureBuffer : public Texture
{

};

class TextureCube : public Texture
{

};

class TextureArray : Object
{};

class Texture1DArray : public TextureArray
{

};

class Texture2DArray : public TextureArray
{

};

class TextureCubeArray : public TextureArray
{

};

class Texture2DMultiSamplerArray : public TextureArray
{

};
} // namespace TwinkleGraphics

#endif