
#include "twTexture.h"

namespace TwinkleGraphics
{
Texture::Texture(bool immutable)
    : Object()
    , _res()
    , _image(nullptr)
    , _sampler(nullptr)
    , _mask(TexParameterMask::TEXPARAMETER_DEFAULT_MASK)
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
    _parameters.wrap_modes[0] = (wrap == WrapMode::NONE ? WrapMode::DEFAULT : wrap);
    _dirty_flag = (TexParameterDirtyFlag)(_dirty_flag | TexParameterDirtyFlag::TEXPARAMETER_WRAP_S_MASK);

    _parameters_dirty = true;
}

template<>
void Texture::SetWrap<WrapParam::WRAP_T>(WrapMode wrap)
{
    if(wrap == _parameters.wrap_modes[1])
        return;
    _parameters.wrap_modes[1] = (wrap == WrapMode::NONE ? WrapMode::DEFAULT : wrap);
    _dirty_flag = (TexParameterDirtyFlag)(_dirty_flag | TexParameterDirtyFlag::TEXPARAMETER_WRAP_T_MASK);

    _parameters_dirty = true;
}

template<>
void Texture::SetWrap<WrapParam::WRAP_R>(WrapMode wrap)
{
    if(wrap == _parameters.wrap_modes[2])
        return;

    _parameters.wrap_modes[2] = (wrap == WrapMode::NONE ? WrapMode::DEFAULT : wrap);
    _dirty_flag = (TexParameterDirtyFlag)(_dirty_flag | TexParameterDirtyFlag::TEXPARAMETER_WRAP_R_MASK);

    _parameters_dirty = true;
}


template<>
void Texture::SetFilter<FilterParam::MIN_FILTER>(FilterMode filter)
{
    if(filter == _parameters.filter_modes[0])
        return;
    _parameters.filter_modes[0] = (filter == FilterMode::NONE ? FilterMode::DEFAULT : filter);
    _dirty_flag = (TexParameterDirtyFlag)(_dirty_flag | TexParameterDirtyFlag::TEXPARAMETER_FILTER_MIN_MASK);

    _parameters_dirty = true;
}

template<>
void Texture::SetFilter<FilterParam::MAG_FILTER>(FilterMode filter)
{
    if(filter == _parameters.filter_modes[1])
        return;
    _parameters.filter_modes[1] = (filter == FilterMode::NONE ? FilterMode::DEFAULT : filter);
    _dirty_flag = (TexParameterDirtyFlag)(_dirty_flag | TexParameterDirtyFlag::TEXPARAMETER_FILTER_MAG_MASK);

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
    else
    {
        _parameters.swizzle_parameter = SwizzleParam::DEFAULT;
        _parameters.swizzle[0] = SwizzleMask::RED;
        _parameters.swizzle[1] = SwizzleMask::GREEN;
        _parameters.swizzle[2] = SwizzleMask::BLUE;
        _parameters.swizzle[3] = SwizzleMask::ALPHA;
    }

    if(dirty)
    {
        _dirty_flag = (TexParameterDirtyFlag)(_dirty_flag | TexParameterDirtyFlag::TEXPARAMETER_SWIZZLE_MASK);
    }
        
    _parameters_dirty |= dirty;
}

void Texture::SetLodBias(LodBiasParam parameter, float32 bias)
{
    bool dirty = _parameters.lod_parameter != parameter;
    _parameters.lod_parameter = parameter;

    if(!dirty && parameter == LodBiasParam::NONE)
        return;

    dirty |= ::fabs(_parameters.lodbias - bias) > glm::epsilon<float32>();
    _parameters.lod_parameter = parameter == LodBiasParam::NONE ? LodBiasParam::DEFAULT : parameter;  
    _parameters.lodbias = parameter == LodBiasParam::NONE ? 0.0f : bias;

    if(dirty)
    {
        _dirty_flag = (TexParameterDirtyFlag)(_dirty_flag | TexParameterDirtyFlag::TEXPARAMETER_LODBIAS_MASK);
    }

    _parameters_dirty |= dirty;
}

void Texture::SetMipMapBaseLevel(MipMapBaseLevelParam parameter, int32 level)
{
    bool dirty = _parameters.baselevel_parameter != parameter;
    _parameters.baselevel_parameter = parameter;

    if(!dirty && parameter == MipMapBaseLevelParam::NONE)
        return;

    dirty |= level != _parameters.mipmap_baselevel;
    if(parameter == MipMapBaseLevelParam::NONE)
        _parameters.mipmap_baselevel = 0;
    else
        _parameters.mipmap_baselevel = level;

    if(dirty)
    {
        _dirty_flag = (TexParameterDirtyFlag)(_dirty_flag | TexParameterDirtyFlag::TEXPARAMETER_MIPMAP_BASE_LEVEL_MASK);        
    }

    _parameters_dirty |= dirty;
}

void Texture::SetMipMapMaxLevel(MipMapMaxLevelParam parameter, int32 level)
{
    bool dirty = _parameters.maxlevel_parameter != parameter;
    _parameters.maxlevel_parameter = parameter;

    if(!dirty && parameter == MipMapMaxLevelParam::NONE)
        return;

    dirty |= level != _parameters.mipmap_maxlevel;
    if(parameter == MipMapMaxLevelParam::NONE)
        _parameters.mipmap_maxlevel = 0;
    else
        _parameters.mipmap_maxlevel = level;

    if(dirty)
    {
        _dirty_flag = (TexParameterDirtyFlag)(_dirty_flag | TexParameterDirtyFlag::TEXPARAMETER_MIPMAP_BASE_LEVEL_MASK);        
    }

    _parameters_dirty |= dirty;
}

void Texture::SetTexBorderColor(TextureBorderColorParam parameter, const vec4& color)
{
    bool dirty = _parameters.bordercolor_parameter != parameter;
    _parameters.bordercolor_parameter = parameter;

    if(!dirty && parameter == TextureBorderColorParam::NONE)
        return;

    dirty |= _parameters.border_color != color;
    if(parameter == TextureBorderColorParam::NONE)
        _parameters.border_color = vec4(0.0f, 0.0f, 0.0f, 0.0f);
    else
        _parameters.border_color = color;    

    if(dirty)
    {
        _dirty_flag = (TexParameterDirtyFlag)(_dirty_flag | TexParameterDirtyFlag::TEXPARAMETER_BORDERCOLOR_MASK);
    }

    _parameters_dirty |= dirty;
}

void Texture::SeTexMinLOD(TextureMinLODParam parameter, int32 lod)
{
    bool dirty = _parameters.min_lod_parameter != parameter;
    _parameters.min_lod_parameter = parameter;

    if(!dirty && parameter == TextureMinLODParam::NONE)
        return;

    dirty |= _parameters.min_lod != lod;
    _parameters.min_lod_parameter = parameter;
    if(parameter == TextureMinLODParam::NONE)
        _parameters.min_lod = 0;
    else
        _parameters.min_lod = lod;

    if(dirty)
    {
        _dirty_flag = (TexParameterDirtyFlag)(_dirty_flag | TexParameterDirtyFlag::TEXPARAMETER_MIN_LOD_MASK);
    }

    _parameters_dirty |= dirty;
}

void Texture::SeTexMaxLOD(TextureMaxLODParam parameter, int32 lod)
{
    bool dirty = _parameters.max_lod_parameter != parameter;
    _parameters.max_lod_parameter = parameter;

    if(!dirty && parameter == TextureMaxLODParam::NONE)
        return;

    dirty |= _parameters.max_lod != lod;
    _parameters.max_lod_parameter = parameter;
    if(parameter == TextureMaxLODParam::NONE)
        _parameters.max_lod = 0;
    else
        _parameters.max_lod = lod;

    if(dirty)
    {
        _dirty_flag = (TexParameterDirtyFlag)(_dirty_flag | TexParameterDirtyFlag::TEXPARAMETER_MAX_LOD_MASK);
    }

    _parameters_dirty |= dirty;
}

void Texture::SetDepthTexCompareMode(DepthTextureCompareMode mode)
{
    if(_parameters.depth_tex_comp_mode == mode)
        return;

    _parameters.depth_tex_comp_mode = mode;
    _dirty_flag = (TexParameterDirtyFlag)(_dirty_flag | TexParameterDirtyFlag::TEXPARAMETER_DEPTH_TEX_COMP_MODE_MASK);

    _parameters_dirty = true;
}

void Texture::SetDepthTexCompareFun(DepthTextureCompareFunc func)
{
    if(_parameters.depth_tex_comp_func == func)
        return;

    _parameters.depth_tex_comp_func = func;
    _dirty_flag = (TexParameterDirtyFlag)(_dirty_flag | TexParameterDirtyFlag::TEXPARAMETER_DEPTH_TEX_COMP_FUNC_MASK);

    _parameters_dirty = true;
}

void Texture::SetDepthStencilMode(DepthStencilTextureMode mode)
{
    if(_parameters.depthstencil_tex_mode == mode)
        return;
    
    _parameters.depthstencil_tex_mode = mode;
    _dirty_flag = (TexParameterDirtyFlag)(_dirty_flag | TexParameterDirtyFlag::TEXPARAMETER_DEPTHSTENCIL_MASK);
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
        ApplyTexBorderColor();

        _parameters_dirty = false;
    }
}

void Texture::ApplyWrapParameter()
{
    if(_sampler != nullptr && _sampler->IsValid())
    {
        

        return;
    }
    
    // wrap
    if (_dirty_flag & TexParameterDirtyFlag::TEXPARAMETER_WRAP_S_MASK != 0)
    {
        glTexParameteri(_res.type, GL_TEXTURE_WRAP_S, (int32)(_parameters.wrap_modes[0]));
    }
    if (_dirty_flag & TexParameterDirtyFlag::TEXPARAMETER_WRAP_T_MASK != 0)
    {
        glTexParameteri(_res.type, GL_TEXTURE_WRAP_T, (int32)(_parameters.wrap_modes[1]));
    }
    if (_dirty_flag & TexParameterDirtyFlag::TEXPARAMETER_WRAP_R_MASK != 0)
    {
        glTexParameteri(_res.type, GL_TEXTURE_WRAP_R, (int32)(_parameters.wrap_modes[2]));
    }
}

void Texture::ApplyFilterParameter()
{
    // filter
    if (_dirty_flag & TexParameterDirtyFlag::TEXPARAMETER_FILTER_MIN_MASK != 0)
    {
        glTexParameteri(_res.type, GL_TEXTURE_MIN_FILTER, (int32)(_parameters.filter_modes[0]));
    }
    if (_dirty_flag & TexParameterDirtyFlag::TEXPARAMETER_FILTER_MAG_MASK != 0)
    {
        glTexParameteri(_res.type, GL_TEXTURE_MAG_FILTER, (int32)(_parameters.filter_modes[1]));
    }
}

void Texture::ApplySwizzleParameter()
{
    // swizzle
    if (_dirty_flag & TexParameterDirtyFlag::TEXPARAMETER_SWIZZLE_MASK != 0)
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
    if (_dirty_flag & TexParameterDirtyFlag::TEXPARAMETER_LODBIAS_MASK != 0)
    {
        glTexParameterf(_res.type, GL_TEXTURE_LOD_BIAS, _parameters.lodbias);
    }
}

void Texture::ApplyDepthTexCompParameter()
{
    if(_dirty_flag & TexParameterDirtyFlag::TEXPARAMETER_DEPTH_TEX_COMP_MODE_MASK != 0)
    {}
    if(_dirty_flag & TexParameterDirtyFlag::TEXPARAMETER_DEPTH_TEX_COMP_FUNC_MASK != 0)
    {}
}

void Texture::ApplyMipMapParameter()
{
    if (_dirty_flag & TexParameterDirtyFlag::TEXPARAMETER_MIPMAP_BASE_LEVEL_MASK != 0)
    {
    }
}

void Texture::ApplyTexLODParameter()
{
    if (_dirty_flag & TexParameterDirtyFlag::TEXPARAMETER_MIPMAP_MAX_LEVEL_MASK != 0)
    {
    }
}

void Texture::ApplyTexBorderColor()
{
    if(_dirty_flag & TexParameterDirtyFlag::TEXPARAMETER_BORDERCOLOR_MASK != 0)
    {
        glTexParameterfv(_res.type, (uint32)TextureBorderColorParam::BORDER_COLOR
                        , glm::value_ptr<float32>(_parameters.border_color));
    }
}

void Texture::ApplyDepthStencilMode()
{
    if (_dirty_flag & TexParameterDirtyFlag::TEXPARAMETER_DEPTHSTENCIL_MASK != 0)
    {
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
