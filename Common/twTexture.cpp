
#include "twTexture.h"

namespace TwinkleGraphics
{
Texture::Texture(bool immutable)
    : Object()
    , _res()
    , _image(nullptr)
    , _immutable(immutable)
    , _initialized(false)
{
    uint32 textures[1] = {0};
    glGenTextures(1, textures);

#ifdef _DEBUG
    GLenum error = glGetError();
    const GLubyte* error_str = glGetString(error);

    assert(textures[0] != 0);
#endif


    _res.id = textures[0];
}

Texture::~Texture()
{
    if(_res.id != 0)
    {
        uint32 id = _res.id;
        glDeleteTextures(1, &id);

#ifdef _DEBUG
        GLenum error = glGetError();
        const GLubyte *error_str = glGetString(error);
#endif
    }
}


template<>
void Texture::SetWrap<WrapParam::WRAP_S>(WrapMode wrap)
{
    _parameters.wrap_s = wrap;
}

template<>
void Texture::SetWrap<WrapParam::WRAP_T>(WrapMode wrap)
{
    _parameters.wrap_t = wrap;
}

template<>
void Texture::SetWrap<WrapParam::WRAP_R>(WrapMode wrap)
{
    _parameters.wrap_r = wrap;
}


template<>
void Texture::SetFilter<FilterParam::MIN_FILTER>(FilterMode filter)
{
    _parameters.filter_min = filter;
}

template<>
void Texture::SetFilter<FilterParam::MAG_FILTER>(FilterMode filter)
{
    _parameters.filter_mag = filter;
}


template<>
void Texture::SetSwizzle<1>(SwizzleParam parameter, Swizzle1 swizzle)
{
    _parameters.swizzle_parameter = parameter;
    _parameters.swizzle[0] = swizzle.mask;
}

template<>
void Texture::SetSwizzle<4>(SwizzleParam parameter, Swizzle4 swizzle)
{
    _parameters.swizzle_parameter = parameter;
    _parameters.swizzle[0] = swizzle.mask[0];
    _parameters.swizzle[1] = swizzle.mask[1];
    _parameters.swizzle[2] = swizzle.mask[2];
    _parameters.swizzle[3] = swizzle.mask[3];
}

void Texture::SetLodBias(LodBiasParam parameter, float32 bias)
{
    _parameters.lodbias = bias;
}

void Texture::InitStorage()
{
    glBindTexture(_res.type, _res.id);

#ifdef _DEBUG
    GLenum error = glGetError();
    const GLubyte* error_str = glGetString(error);
#endif

}


/*------------------------------Texture1D----------------------------- */


void Texture1D::InitStorage()
{
    Texture::InitStorage();

    const ImageData& image_source = _image->GetImageSource();
    if(_immutable)
    {
        glTexStorage1D(_res.type,
                       image_source.mipLevels,
                       image_source.internalFormat,
                       image_source.mip[0].width);

        for (int32 level = 0, miplevels = image_source.mipLevels; level < miplevels; ++level)
        {
            glTexSubImage1D(_res.type,
                            level,
                            0,
                            image_source.mip[level].width,
                            image_source.format, 
                            image_source.type,
                            image_source.mip[level].data);
        }
    }
    else
    {
        for (int32 level = 0, miplevels = image_source.mipLevels; level < miplevels; ++level)
        {
            glTexImage1D(_res.type,
                         level,
                         image_source.internalFormat,
                         image_source.mip[level].width,
                         0,
                         image_source.format,
                         image_source.type,
                         image_source.mip[level].data);
        }
    }
}



/*------------------------------Texture2D----------------------------- */


void Texture2D::InitStorage()
{
    Texture::InitStorage();

    const ImageData& image_source = _image->GetImageSource();
    if(_immutable)
    {
        glTexStorage2D(_res.type, image_source.mipLevels, 
                        image_source.internalFormat, 
                        image_source.mip[0].width, 
                        image_source.mip[0].height
        );

        for (int32 level = 0, miplevels = image_source.mipLevels; level < miplevels; ++level)
        {
            glTexSubImage2D(_res.type,
                            level,
                            0, 0,
                            image_source.mip[level].width,
                            image_source.mip[level].height,
                            image_source.format,
                            image_source.type,
                            image_source.mip[level].data);
        }
    }
    else
    {
        for (int32 level = 0, miplevels = image_source.mipLevels; level < miplevels; ++level)
        {
            glTexImage2D(_res.type,
                         level,
                         image_source.internalFormat,
                         image_source.mip[level].width,
                         image_source.mip[level].height,
                         0,
                         image_source.format,
                         image_source.type,
                         image_source.mip[level].data);
        }
    }
}

void Texture2DMultiSample::InitStorage()
{}

void Texture3D::InitStorage()
{
}

void TextureRectangle::InitStorage()
{
}

void TextureBuffer::InitStorage()
{
}

void TextureCube::InitStorage()
{
}

void Texture1DArray::InitStorage()
{
}

void TextureCubeArray::InitStorage()
{
}

void Texture2DMultiSampleArray::InitStorage()
{
}

} // namespace TwinkleGraphics
