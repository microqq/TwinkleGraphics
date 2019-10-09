
#include "twTexture.h"

namespace TwinkleGraphics
{
Texture::Texture(bool immutable)
    : Object()
    , _res()
    , _image(nullptr)
    , _sampler(nullptr)
    , _data(nullptr)
    , _mask(TexParameterMask::TEXPARAMETER_DEFAULT_MASK)
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
    if(_data != nullptr)
    {
        _data->ReleaseMipData();
        SAFE_DEL(_data);
    }

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
}

template<>
void Texture::SetWrap<WrapParam::WRAP_T>(WrapMode wrap)
{
    if(wrap == _parameters.wrap_modes[1])
        return;
    _parameters.wrap_modes[1] = (wrap == WrapMode::NONE ? WrapMode::DEFAULT : wrap);
    _dirty_flag = (TexParameterDirtyFlag)(_dirty_flag | TexParameterDirtyFlag::TEXPARAMETER_WRAP_T_MASK);
}

template<>
void Texture::SetWrap<WrapParam::WRAP_R>(WrapMode wrap)
{
    if(wrap == _parameters.wrap_modes[2])
        return;

    _parameters.wrap_modes[2] = (wrap == WrapMode::NONE ? WrapMode::DEFAULT : wrap);
    _dirty_flag = (TexParameterDirtyFlag)(_dirty_flag | TexParameterDirtyFlag::TEXPARAMETER_WRAP_R_MASK);
}


template<>
void Texture::SetFilter<FilterParam::MIN_FILTER>(FilterMode filter)
{
    if(filter == _parameters.filter_modes[0])
        return;
    _parameters.filter_modes[0] = (filter == FilterMode::NONE ? FilterMode::DEFAULT : filter);
    _dirty_flag = (TexParameterDirtyFlag)(_dirty_flag | TexParameterDirtyFlag::TEXPARAMETER_FILTER_MIN_MASK);
}

template<>
void Texture::SetFilter<FilterParam::MAG_FILTER>(FilterMode filter)
{
    if(filter == _parameters.filter_modes[1])
        return;
    _parameters.filter_modes[1] = (filter == FilterMode::NONE ? FilterMode::DEFAULT : filter);
    _dirty_flag = (TexParameterDirtyFlag)(_dirty_flag | TexParameterDirtyFlag::TEXPARAMETER_FILTER_MAG_MASK);
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
}

void Texture::SetDepthTexCompareMode(DepthTextureCompareMode mode)
{
    if(_parameters.depth_tex_comp_mode == mode)
        return;

    _parameters.depth_tex_comp_mode = mode;
    _dirty_flag = (TexParameterDirtyFlag)(_dirty_flag | TexParameterDirtyFlag::TEXPARAMETER_DEPTH_TEX_COMP_MODE_MASK);
}

void Texture::SetDepthTexCompareFun(DepthTextureCompareFunc func)
{
    if(_parameters.depth_tex_comp_func == func)
        return;

    _parameters.depth_tex_comp_func = func;
    _dirty_flag = (TexParameterDirtyFlag)(_dirty_flag | TexParameterDirtyFlag::TEXPARAMETER_DEPTH_TEX_COMP_FUNC_MASK);
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
    ApplyWrapParameter();
    ApplyFilterParameter();
    ApplySwizzleParameter();
    ApplyLodBias();
    ApplyTexBorderColor();
    ApplyDepthStencilMode();
    ApplyDepthTexCompParameter();
    ApplyMipMapParameter();
    ApplyTexLODParameter();
}

void Texture::ApplyWrapParameter()
{
    bool wrap_s_mask = (_mask & TexParameterMask::TEXPARAMETER_WRAP_S_MASK) != 0;
    bool wrap_t_mask = (_mask & TexParameterMask::TEXPARAMETER_WRAP_T_MASK) != 0;
    bool wrap_r_mask = (_mask & TexParameterMask::TEXPARAMETER_WRAP_R_MASK) != 0;

    bool use_sampler = _sampler != nullptr && _sampler->IsValid();
    uint32 sampler_id = -1;
    if(use_sampler)
        sampler_id = _sampler->GetRenderRes().id;

    // wrap
    if (wrap_s_mask && 
        (_dirty_flag & TexParameterDirtyFlag::TEXPARAMETER_WRAP_S_MASK) != 0)
    {
        if(use_sampler)
            glSamplerParameteri(sampler_id, GL_TEXTURE_WRAP_S, (int32)(_parameters.wrap_modes[0]));
        else
            glTexParameteri(_res.type, GL_TEXTURE_WRAP_S, (int32)(_parameters.wrap_modes[0]));

        _dirty_flag = (TexParameterDirtyFlag)(
            (uint32)_dirty_flag & (!((uint32)TexParameterDirtyFlag::TEXPARAMETER_WRAP_S_MASK))
            );
    }
    if (wrap_t_mask && 
        (_dirty_flag & TexParameterDirtyFlag::TEXPARAMETER_WRAP_T_MASK) != 0)
    {
        if (use_sampler)
            glSamplerParameteri(sampler_id, GL_TEXTURE_WRAP_T, (int32)(_parameters.wrap_modes[1]));
        else
            glTexParameteri(_res.type, GL_TEXTURE_WRAP_T, (int32)(_parameters.wrap_modes[1]));

        _dirty_flag = (TexParameterDirtyFlag)(
            (uint32)_dirty_flag & (!((uint32)TexParameterDirtyFlag::TEXPARAMETER_WRAP_T_MASK))
            );
    }
    if (wrap_r_mask &&
        (_dirty_flag & TexParameterDirtyFlag::TEXPARAMETER_WRAP_R_MASK) != 0)
    {
        if (use_sampler)
            glSamplerParameteri(sampler_id, GL_TEXTURE_WRAP_S, (int32)(_parameters.wrap_modes[2]));
        else
            glTexParameteri(_res.type, GL_TEXTURE_WRAP_R, (int32)(_parameters.wrap_modes[2]));

        _dirty_flag = (TexParameterDirtyFlag)(
            (uint32)_dirty_flag & (!((uint32)TexParameterDirtyFlag::TEXPARAMETER_WRAP_R_MASK))
            );
    }
}

void Texture::ApplyFilterParameter()
{
    bool filter_min_mask = (_mask & TexParameterMask::TEXPARAMETER_FILTER_MIN_MASK) != 0;
    bool filter_mag_mask = (_mask & TexParameterMask::TEXPARAMETER_FILTER_MAG_MASK) != 0;

    bool use_sampler = _sampler != nullptr && _sampler->IsValid();
    uint32 sampler_id = -1;
    if(use_sampler)
        sampler_id = _sampler->GetRenderRes().id;

    // Minification filter
    if (filter_min_mask && (_dirty_flag & TexParameterDirtyFlag::TEXPARAMETER_FILTER_MIN_MASK) != 0)
    {
        if(use_sampler)
            glSamplerParameteri(sampler_id, GL_TEXTURE_MIN_FILTER, (int32)(_parameters.filter_modes[0]));
        else
            glTexParameteri(_res.type, GL_TEXTURE_MIN_FILTER, (int32)(_parameters.filter_modes[0]));

        _dirty_flag = (TexParameterDirtyFlag)(
            (uint32)_dirty_flag & (!((uint32)TexParameterDirtyFlag::TEXPARAMETER_FILTER_MIN_MASK)));
    }

    // Magnification filter
    if (filter_mag_mask && (_dirty_flag & TexParameterDirtyFlag::TEXPARAMETER_FILTER_MAG_MASK) != 0)
    {
        if (use_sampler)
            glSamplerParameteri(sampler_id, GL_TEXTURE_MAG_FILTER, (int32)(_parameters.filter_modes[1]));
        else
            glTexParameteri(_res.type, GL_TEXTURE_MAG_FILTER, (int32)(_parameters.filter_modes[1]));

        _dirty_flag = (TexParameterDirtyFlag)(
            (uint32)_dirty_flag & (!((uint32)TexParameterDirtyFlag::TEXPARAMETER_FILTER_MAG_MASK)));
    }
}

void Texture::ApplySwizzleParameter()
{
    bool swizzle_mask = (_mask & TexParameterMask::TEXPARAMETER_SWIZZLE_MASK) != 0;

    // swizzle
    if (swizzle_mask && (_dirty_flag & TexParameterDirtyFlag::TEXPARAMETER_SWIZZLE_MASK) != 0)
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

        _dirty_flag = (TexParameterDirtyFlag)(
            (uint32)_dirty_flag & (!((uint32)TexParameterDirtyFlag::TEXPARAMETER_SWIZZLE_MASK)));
    }
}

void Texture::ApplyLodBias()
{
    bool lodbias_mask = (_mask & TexParameterMask::TEXPARAMETER_LODBIAS_MASK) != 0;

    bool use_sampler = _sampler != nullptr && _sampler->IsValid();
    uint32 sampler_id = -1;
    if (use_sampler)
        sampler_id = _sampler->GetRenderRes().id;

    // lod bias
    if (lodbias_mask && _dirty_flag & TexParameterDirtyFlag::TEXPARAMETER_LODBIAS_MASK != 0)
    {
        if(use_sampler)
            glSamplerParameteri(sampler_id, GL_TEXTURE_LOD_BIAS, _parameters.lodbias);
        else
            glTexParameterf(_res.type, GL_TEXTURE_LOD_BIAS, _parameters.lodbias);

        _dirty_flag = (TexParameterDirtyFlag)(
            (uint32)_dirty_flag & (!((uint32)TexParameterDirtyFlag::TEXPARAMETER_LODBIAS_MASK)));
    }
}

void Texture::ApplyDepthTexCompParameter()
{
    bool depth_tex_mode_mask = (_mask & TexParameterMask::TEXPARAMETER_DEPTH_TEX_COMP_MODE_MASK) != 0;
    bool depth_tex_func_mask = (_mask & TexParameterMask::TEXPARAMETER_DEPTH_TEX_COMP_FUNC_MASK) != 0;

    bool use_sampler = _sampler != nullptr && _sampler->IsValid();
    uint32 sampler_id = -1;
    if (use_sampler)
        sampler_id = _sampler->GetRenderRes().id;

    // depth tex compare mode
    if (depth_tex_mode_mask && (_dirty_flag & TexParameterDirtyFlag::TEXPARAMETER_DEPTH_TEX_COMP_MODE_MASK) != 0)
    {
        _dirty_flag = (TexParameterDirtyFlag)(
            (uint32)_dirty_flag & (!((uint32)TexParameterDirtyFlag::TEXPARAMETER_DEPTH_TEX_COMP_MODE_MASK)));
    }

    // depth tex compare func
    if (depth_tex_func_mask && (_dirty_flag & TexParameterDirtyFlag::TEXPARAMETER_DEPTH_TEX_COMP_FUNC_MASK) != 0)
    {
        _dirty_flag = (TexParameterDirtyFlag)(
            (uint32)_dirty_flag & (!((uint32)TexParameterDirtyFlag::TEXPARAMETER_DEPTH_TEX_COMP_FUNC_MASK)));
    }
}

void Texture::ApplyMipMapParameter()
{
    bool mipmap_base_level_mask = (_mask & TexParameterMask::TEXPARAMETER_MIPMAP_BASE_LEVEL_MASK) != 0;
    bool mipmap_max_level_mask = (_mask & TexParameterMask::TEXPARAMETER_MIPMAP_MAX_LEVEL_MASK) != 0;

    bool use_sampler = _sampler != nullptr && _sampler->IsValid();
    uint32 sampler_id = -1;
    if (use_sampler)
        sampler_id = _sampler->GetRenderRes().id;

    if (mipmap_base_level_mask && (_dirty_flag & TexParameterDirtyFlag::TEXPARAMETER_MIPMAP_BASE_LEVEL_MASK) != 0)
    {

        _dirty_flag = (TexParameterDirtyFlag)(
            (uint32)_dirty_flag & (!((uint32)TexParameterDirtyFlag::TEXPARAMETER_MIPMAP_BASE_LEVEL_MASK)));
    }

    if (mipmap_max_level_mask && (_dirty_flag & TexParameterDirtyFlag::TEXPARAMETER_MIPMAP_MAX_LEVEL_MASK) != 0)
    {

        _dirty_flag = (TexParameterDirtyFlag)(
            (uint32)_dirty_flag & (!((uint32)TexParameterDirtyFlag::TEXPARAMETER_MIPMAP_MAX_LEVEL_MASK)));
    }
}

void Texture::ApplyTexLODParameter()
{
    bool tex_min_lod_mask = (_mask & TexParameterMask::TEXPARAMETER_MIN_LOD_MASK) != 0;
    bool tex_max_lod_mask = (_mask & TexParameterMask::TEXPARAMETER_MAX_LOD_MASK) != 0;

    bool use_sampler = _sampler != nullptr && _sampler->IsValid();
    uint32 sampler_id = -1;
    if (use_sampler)
        sampler_id = _sampler->GetRenderRes().id;

    if (tex_min_lod_mask && (_dirty_flag & TexParameterDirtyFlag::TEXPARAMETER_MIN_LOD_MASK) != 0)
    {
        _dirty_flag = (TexParameterDirtyFlag)(
            (uint32)_dirty_flag & (!((uint32)TexParameterDirtyFlag::TEXPARAMETER_MIN_LOD_MASK)));
    }

    if (tex_max_lod_mask && (_dirty_flag & TexParameterDirtyFlag::TEXPARAMETER_MAX_LOD_MASK) != 0)
    {
        _dirty_flag = (TexParameterDirtyFlag)(
            (uint32)_dirty_flag & (!((uint32)TexParameterDirtyFlag::TEXPARAMETER_MAX_LOD_MASK)));
    }
}

void Texture::ApplyTexBorderColor()
{
    bool border_color_mask = (_mask & TexParameterMask::TEXPARAMETER_BORDERCOLOR_MASK) != 0;

    bool use_sampler = _sampler != nullptr && _sampler->IsValid();
    uint32 sampler_id = -1;
    if (use_sampler)
        sampler_id = _sampler->GetRenderRes().id;

    if (border_color_mask && (_dirty_flag & TexParameterDirtyFlag::TEXPARAMETER_BORDERCOLOR_MASK) != 0)
    {
        if(use_sampler)
            glSamplerParameterfv(sampler_id, GL_TEXTURE_BORDER_COLOR
                        , glm::value_ptr<float32>(_parameters.border_color));
        else
            glTexParameterfv(_res.type, (uint32)TextureBorderColorParam::BORDER_COLOR
                        , glm::value_ptr<float32>(_parameters.border_color));

        _dirty_flag = (TexParameterDirtyFlag)(
            (uint32)_dirty_flag & (!((uint32)TexParameterDirtyFlag::TEXPARAMETER_BORDERCOLOR_MASK)));
    }
}

void Texture::ApplyDepthStencilMode()
{
    bool depthstencil_mask = (_mask & TexParameterMask::TEXPARAMETER_DEPTHSTENCIL_MASK) != 0;

    bool use_sampler = _sampler != nullptr && _sampler->IsValid();
    uint32 sampler_id = -1;
    if (use_sampler)
        sampler_id = _sampler->GetRenderRes().id;

    if (depthstencil_mask && (_dirty_flag & TexParameterDirtyFlag::TEXPARAMETER_DEPTHSTENCIL_MASK) != 0)
    {

        _dirty_flag = (TexParameterDirtyFlag)(
            (uint32)_dirty_flag & (!((uint32)TexParameterDirtyFlag::TEXPARAMETER_DEPTHSTENCIL_MASK)));
    }
}

/*------------------------------Texture1D----------------------------- */


void Texture1D::InitStorage()
{
    Texture::InitStorage();

    ImageData* destimage = nullptr;
    if(_image != nullptr)
    {
        const ImageData& source = _image->GetImageSource();
        destimage = new ImageData(source);
    }
    else if(_data != nullptr)
    {
        destimage = _data;
    }
    
    if(destimage != nullptr)
    {
        if(_immutable)
        {
            glTexStorage1D(_res.type,
                        destimage->mipLevels,
                        destimage->internalFormat,
                        destimage->mip[0].width);

            for (int32 level = 0, mipLevels = destimage->mipLevels; level < mipLevels; ++level)
            {
                glTexSubImage1D(_res.type,
                                level,
                                0,
                                destimage->mip[level].width,
                                destimage->format, 
                                destimage->type,
                                destimage->mip[level].data);
            }
        }
        else
        {
            for (int32 level = 0, mipLevels = destimage->mipLevels; level < mipLevels; ++level)
            {
                glTexImage1D(_res.type,
                            level,
                            destimage->internalFormat,
                            destimage->mip[level].width,
                            0,
                            destimage->format,
                            destimage->type,
                            destimage->mip[level].data);
            }
        }
    }
}



/*------------------------------Texture2D----------------------------- */


void Texture2D::InitStorage()
{
    Texture::InitStorage();

    ImageData* destimage = nullptr;
    if(_image != nullptr)
    {
        const ImageData& source = _image->GetImageSource();
        destimage = new ImageData(source);
    }
    else if(_data != nullptr)
    {
        destimage = _data;
    }
    
    if(destimage != nullptr)
    {
        const ImageData& image_source = _image->GetImageSource();
        if(_immutable)
        {
            glTexStorage2D(_res.type, destimage->mipLevels, 
                            destimage->internalFormat, 
                            destimage->mip[0].width, 
                            destimage->mip[0].height
            );

            for (int32 level = 0, mipLevels = destimage->mipLevels; level < mipLevels; ++level)
            {
                glTexSubImage2D(_res.type,
                                level,
                                0, 0,
                                destimage->mip[level].width,
                                destimage->mip[level].height,
                                destimage->format,
                                destimage->type,
                                destimage->mip[level].data);
            }
        }
        else
        {
            for (int32 level = 0, mipLevels = destimage->mipLevels; level < mipLevels; ++level)
            {
                glTexImage2D(_res.type,
                            level,
                            destimage->internalFormat,
                            destimage->mip[level].width,
                            destimage->mip[level].height,
                            0,
                            destimage->format,
                            destimage->type,
                            destimage->mip[level].data);
            }
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
