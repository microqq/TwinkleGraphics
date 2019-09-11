
#include "twTexture.h"

namespace TwinkleGraphics
{
Texture::Texture(bool immutable)
    : Object()
    , _res()
    , _image(nullptr)
    , _immutable(immutable)
    , _initialized(false)
    , _parameters_dirty(true)
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
    if(wrap == _parameters.wrap_modes[0])
        return;
    _parameters.wrap_modes[0] = wrap;
    _parameters_dirty = true;
}

template<>
void Texture::SetWrap<WrapParam::WRAP_T>(WrapMode wrap)
{
    if(wrap == _parameters.wrap_modes[1])
        return;
    _parameters.wrap_modes[1] = wrap;
    _parameters_dirty = true;
}

template<>
void Texture::SetWrap<WrapParam::WRAP_R>(WrapMode wrap)
{
    if(wrap == _parameters.wrap_modes[2])
        return;
    _parameters.wrap_modes[2] = wrap;
    _parameters_dirty = true;
}


template<>
void Texture::SetFilter<FilterParam::MIN_FILTER>(FilterMode filter)
{
    if(filter == _parameters.filter_modes[0])
        return;
    _parameters.filter_modes[0] = filter;
    _parameters_dirty = true;
}

template<>
void Texture::SetFilter<FilterParam::MAG_FILTER>(FilterMode filter)
{
    if(filter == _parameters.filter_modes[1])
        return;
    _parameters.filter_modes[1] = filter;
    _parameters_dirty = true;
}


template<>
void Texture::SetSwizzle<1>(SwizzleParam parameter, Swizzle1 swizzle)
{
    _parameters.swizzle_parameter = parameter;
    _parameters.swizzle[0] = swizzle.mask;
    _parameters_dirty = true;
}

template<>
void Texture::SetSwizzle<4>(SwizzleParam parameter, Swizzle4 swizzle)
{
    _parameters.swizzle_parameter = parameter;
    _parameters.swizzle[0] = swizzle.mask[0];
    _parameters.swizzle[1] = swizzle.mask[1];
    _parameters.swizzle[2] = swizzle.mask[2];
    _parameters.swizzle[3] = swizzle.mask[3];
    _parameters_dirty = true;
}

void Texture::SetSwizzle(SwizzleParam parameter, SwizzleMask* masks)
{
    bool dirty = _parameters.swizzle_parameter != parameter;
    _parameters.swizzle_parameter = parameter;

    if(!dirty && parameter == SwizzleParam::NONE)
        return;

    int32 val = (uint32)_parameters.swizzle_parameter - (uint32)SwizzleParam::SWIZZLE_R;
    if(val >= 0 && val < 4)
    {
        dirty = masks[val] != _parameters.swizzle[val];
        _parameters.swizzle[val] = masks[val];
    }
    else if(val == 4)
    {        
        dirty |= _parameters.swizzle[0] != masks[0];
        dirty |= _parameters.swizzle[1] != masks[1];
        dirty |= _parameters.swizzle[2] != masks[2];
        dirty |= _parameters.swizzle[3] != masks[3];

        _parameters.swizzle[0] = masks[0];
        _parameters.swizzle[1] = masks[1];
        _parameters.swizzle[2] = masks[2];
        _parameters.swizzle[3] = masks[3];
    }
    
    _parameters_dirty |= dirty;
}

void Texture::SetLodBias(LodBiasParam parameter, float32 bias)
{
    bool dirty = _parameters.lod_parameter == parameter;
    _parameters.lod_parameter = parameter;

    if(!dirty && parameter == LodBiasParam::NONE)
        return;

    dirty |= ::fabs(_parameters.lodbias - bias) > glm::epsilon<float32>();
    _parameters.lodbias = bias;
    _parameters_dirty |= dirty;
}

void Texture::InitStorage()
{
    glBindTexture(_res.type, _res.id);

#ifdef _DEBUG
    GLenum error = glGetError();
    const GLubyte* error_str = glGetString(error);
#endif

}

void Texture::ApplyTexParameters()
{
    if (_parameters_dirty)
    {
        ApplyWrapParameter();
        ApplyFilterParameter();
        ApplySwizzleParameter();
        ApplyLodBias();

        _parameters_dirty = false;
    }
}

void Texture::ApplyWrapParameter()
{
    // wrap
    if (_parameters.wrap_modes[0] != WrapMode::NONE)
    {
        glTexParameteri(_res.type, GL_TEXTURE_WRAP_S, (int32)(_parameters.wrap_modes[0]));
    }
    if (_parameters.wrap_modes[1] != WrapMode::NONE)
    {
        glTexParameteri(_res.type, GL_TEXTURE_WRAP_T, (int32)(_parameters.wrap_modes[1]));
    }
    if (_parameters.wrap_modes[2] != WrapMode::NONE)
    {
        glTexParameteri(_res.type, GL_TEXTURE_WRAP_R, (int32)(_parameters.wrap_modes[2]));
    }
}

void Texture::ApplyFilterParameter()
{
    // filter
    if (_parameters.filter_modes[0] != FilterMode::NONE)
    {
        glTexParameteri(_res.type, GL_TEXTURE_MIN_FILTER, (int32)(_parameters.filter_modes[0]));
    }
    if (_parameters.filter_modes[1] != FilterMode::NONE)
    {
        glTexParameteri(_res.type, GL_TEXTURE_MAG_FILTER, (int32)(_parameters.filter_modes[1]));
    }
}

void Texture::ApplySwizzleParameter()
{
    // swizzle
    if (_parameters.swizzle_parameter != SwizzleParam::NONE)
    {
        uint32 swizzle_param = uint32(_parameters.swizzle_parameter);
        int32 index = swizzle_param - (uint32)(SwizzleParam::SWIZZLE_R);
        if (index == 4)
        {
            glTexParameteriv(_res.type, swizzle_param, reinterpret_cast<const int32 *>(_parameters.swizzle));
        }
        else
        {
            glTexParameteri(_res.type, swizzle_param, (int32)(_parameters.swizzle[index]));
        }
    }
}

void Texture::ApplyLodBias()
{
    // lod bias
    if (_parameters.lod_parameter != LodBiasParam::NONE)
    {
        glTexParameterf(_res.type, GL_TEXTURE_LOD_BIAS, _parameters.lodbias);
    }
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
