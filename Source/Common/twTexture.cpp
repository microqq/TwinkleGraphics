
#include "twTexture.h"

namespace TwinkleGraphics
{
Texture::Texture(bool immutable, bool genMipMap)
    : Object()
    , _res()
    , _image(nullptr)
    , _sampler(nullptr)
    , _mask(TexParameterMask::TEXPARAMETER_DEFAULT_MASK)
    , _dirtyFlag(TexParameterMask::TEXPARAMETER_DEFAULT_DIRTY_FLAG)
    , _internalformat(GL_NONE)
    , _format(GL_NONE)
    , _miplevels(0)
    , _width(-1)
    , _height(-1)
    , _depth(-1)
    , _immutable(immutable)
    , _generateMipMap(genMipMap)
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
        glBindTexture(_res.type, 0);

        uint32 id = _res.id;
        glDeleteTextures(1, &id);

#ifdef _DEBUG
        GLenum error = glGetError();
        const GLubyte *error_str = glGetString(error);
#endif
    }
}


template<>
void __TWCOMExport Texture::SetWrap<WrapParam::WRAP_S>(WrapMode wrap)
{
    if(wrap == _parameters.wrapModes[0])
        return;
    _parameters.wrapModes[0] = (wrap == WrapMode::NONE ? WrapMode::DEFAULT : wrap);
    _dirtyFlag = (TexParameterDirtyFlag)(_dirtyFlag | TexParameterDirtyFlag::TEXPARAMETER_WRAP_S_MASK);
}

template<>
void __TWCOMExport Texture::SetWrap<WrapParam::WRAP_T>(WrapMode wrap)
{
    if(wrap == _parameters.wrapModes[1])
        return;
    _parameters.wrapModes[1] = (wrap == WrapMode::NONE ? WrapMode::DEFAULT : wrap);
    _dirtyFlag = (TexParameterDirtyFlag)(_dirtyFlag | TexParameterDirtyFlag::TEXPARAMETER_WRAP_T_MASK);
}

template<>
void __TWCOMExport Texture::SetWrap<WrapParam::WRAP_R>(WrapMode wrap)
{
    if(wrap == _parameters.wrapModes[2])
        return;

    _parameters.wrapModes[2] = (wrap == WrapMode::NONE ? WrapMode::DEFAULT : wrap);
    _dirtyFlag = (TexParameterDirtyFlag)(_dirtyFlag | TexParameterDirtyFlag::TEXPARAMETER_WRAP_R_MASK);
}


template<>
void __TWCOMExport Texture::SetFilter<FilterParam::MIN_FILTER>(FilterMode filter)
{
    if(filter == _parameters.filterModes[0])
        return;
    _parameters.filterModes[0] = (filter == FilterMode::NONE ? FilterMode::DEFAULT : filter);
    _dirtyFlag = (TexParameterDirtyFlag)(_dirtyFlag | TexParameterDirtyFlag::TEXPARAMETER_FILTER_MIN_MASK);
}

template<>
void __TWCOMExport Texture::SetFilter<FilterParam::MAG_FILTER>(FilterMode filter)
{
    if(filter == _parameters.filterModes[1])
        return;
    _parameters.filterModes[1] = (filter == FilterMode::NONE ? FilterMode::DEFAULT : filter);
    _dirtyFlag = (TexParameterDirtyFlag)(_dirtyFlag | TexParameterDirtyFlag::TEXPARAMETER_FILTER_MAG_MASK);
}


template<>
void __TWCOMExport Texture::SetSwizzle<1>(SwizzleParam parameter, Swizzle1 swizzle)
{
    _parameters.swizzleParameter = parameter;
    _parameters.swizzle[0] = swizzle.mask;
}

template<>
void __TWCOMExport Texture::SetSwizzle<4>(SwizzleParam parameter, Swizzle4 swizzle)
{
    _parameters.swizzleParameter = parameter;
    _parameters.swizzle[0] = swizzle.mask[0];
    _parameters.swizzle[1] = swizzle.mask[1];
    _parameters.swizzle[2] = swizzle.mask[2];
    _parameters.swizzle[3] = swizzle.mask[3];
}

void Texture::SetSwizzle(SwizzleParam parameter, SwizzleMask* masks)
{
    bool dirty = _parameters.swizzleParameter != parameter;
    _parameters.swizzleParameter = parameter;

    if(!dirty && parameter == SwizzleParam::NONE)
        return;

    int32 val = (uint32)_parameters.swizzleParameter - (uint32)SwizzleParam::SWIZZLE_R;
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
        _parameters.swizzleParameter = SwizzleParam::DEFAULT;
        _parameters.swizzle[0] = SwizzleMask::RED;
        _parameters.swizzle[1] = SwizzleMask::GREEN;
        _parameters.swizzle[2] = SwizzleMask::BLUE;
        _parameters.swizzle[3] = SwizzleMask::ALPHA;
    }

    if(dirty)
    {
        _dirtyFlag = (TexParameterDirtyFlag)(_dirtyFlag | TexParameterDirtyFlag::TEXPARAMETER_SWIZZLE_MASK);
    }        
}

void Texture::SetLodBias(LodBiasParam parameter, float32 bias)
{
    bool dirty = _parameters.lodParameter != parameter;
    _parameters.lodParameter = parameter;

    if(!dirty && parameter == LodBiasParam::NONE)
        return;

    dirty |= ::fabs(_parameters.lodbias - bias) > glm::epsilon<float32>();
    _parameters.lodParameter = parameter == LodBiasParam::NONE ? LodBiasParam::DEFAULT : parameter;  
    _parameters.lodbias = parameter == LodBiasParam::NONE ? 0.0f : bias;

    if(dirty)
    {
        _dirtyFlag = (TexParameterDirtyFlag)(_dirtyFlag | TexParameterDirtyFlag::TEXPARAMETER_LODBIAS_MASK);
    }
}

void Texture::SetMipMapBaseLevel(MipMapBaseLevelParam parameter, int32 level)
{
    bool dirty = _parameters.baselevelParameter != parameter;
    _parameters.baselevelParameter = parameter;

    if(!dirty && parameter == MipMapBaseLevelParam::NONE)
        return;

    dirty |= level != _parameters.mipmapBaselevel;
    if(parameter == MipMapBaseLevelParam::NONE)
        _parameters.mipmapBaselevel = 0;
    else
        _parameters.mipmapBaselevel = level;

    if(dirty)
    {
        _dirtyFlag = (TexParameterDirtyFlag)(_dirtyFlag | TexParameterDirtyFlag::TEXPARAMETER_MIPMAP_BASE_LEVEL_MASK);        
    }
}

void Texture::SetMipMapMaxLevel(MipMapMaxLevelParam parameter, int32 level)
{
    bool dirty = _parameters.maxlevelParameter != parameter;
    _parameters.maxlevelParameter = parameter;

    if(!dirty && parameter == MipMapMaxLevelParam::NONE)
        return;

    dirty |= level != _parameters.mipmapMaxlevel;
    if(parameter == MipMapMaxLevelParam::NONE)
        _parameters.mipmapMaxlevel = 0;
    else
        _parameters.mipmapMaxlevel = level;

    if(dirty)
    {
        _dirtyFlag = (TexParameterDirtyFlag)(_dirtyFlag | TexParameterDirtyFlag::TEXPARAMETER_MIPMAP_BASE_LEVEL_MASK);        
    }
}

void Texture::SetTexBorderColor(TextureBorderColorParam parameter, const vec4& color)
{
    bool dirty = _parameters.bordercolorParameter != parameter;
    _parameters.bordercolorParameter = parameter;

    if(!dirty && parameter == TextureBorderColorParam::NONE)
        return;

    dirty |= _parameters.borderColor != color;
    if(parameter == TextureBorderColorParam::NONE)
        _parameters.borderColor = vec4(0.0f, 0.0f, 0.0f, 0.0f);
    else
        _parameters.borderColor = color;    

    if(dirty)
    {
        _dirtyFlag = (TexParameterDirtyFlag)(_dirtyFlag | TexParameterDirtyFlag::TEXPARAMETER_BORDERCOLOR_MASK);
    }
}

void Texture::SeTexMinLOD(TextureMinLODParam parameter, int32 lod)
{
    bool dirty = _parameters.minLodParameter != parameter;
    _parameters.minLodParameter = parameter;

    if(!dirty && parameter == TextureMinLODParam::NONE)
        return;

    dirty |= _parameters.minLod != lod;
    _parameters.minLodParameter = parameter;
    if(parameter == TextureMinLODParam::NONE)
        _parameters.minLod = 0;
    else
        _parameters.minLod = lod;

    if(dirty)
    {
        _dirtyFlag = (TexParameterDirtyFlag)(_dirtyFlag | TexParameterDirtyFlag::TEXPARAMETER_MIN_LOD_MASK);
    }
}

void Texture::SeTexMaxLOD(TextureMaxLODParam parameter, int32 lod)
{
    bool dirty = _parameters.maxLodParameter != parameter;
    _parameters.maxLodParameter = parameter;

    if(!dirty && parameter == TextureMaxLODParam::NONE)
        return;

    dirty |= _parameters.maxLod != lod;
    _parameters.maxLodParameter = parameter;
    if(parameter == TextureMaxLODParam::NONE)
        _parameters.maxLod = 0;
    else
        _parameters.maxLod = lod;

    if(dirty)
    {
        _dirtyFlag = (TexParameterDirtyFlag)(_dirtyFlag | TexParameterDirtyFlag::TEXPARAMETER_MAX_LOD_MASK);
    }
}

void Texture::SetDepthTexCompareMode(DepthTextureCompareMode mode)
{
    if(_parameters.depthTexCompMode == mode)
        return;

    _parameters.depthTexCompMode = mode;
    _dirtyFlag = (TexParameterDirtyFlag)(_dirtyFlag | TexParameterDirtyFlag::TEXPARAMETER_DEPTH_TEX_COMP_MODE_MASK);
}

void Texture::SetDepthTexCompareFun(DepthTextureCompareFunc func)
{
    if(_parameters.depthTexCompFunc == func)
        return;

    _parameters.depthTexCompFunc = func;
    _dirtyFlag = (TexParameterDirtyFlag)(_dirtyFlag | TexParameterDirtyFlag::TEXPARAMETER_DEPTH_TEX_COMP_FUNC_MASK);
}

void Texture::SetDepthStencilMode(DepthStencilTextureMode mode)
{
    if(_parameters.depthstencilTexMode == mode)
        return;
    
    _parameters.depthstencilTexMode = mode;
    _dirtyFlag = (TexParameterDirtyFlag)(_dirtyFlag | TexParameterDirtyFlag::TEXPARAMETER_DEPTHSTENCIL_MASK);
}

void Texture::SetAnistropic(float32 anistropic, bool maxAnistropic)
{
    if(anistropic <= 0.0f && !maxAnistropic)
    {
        return;
    }

    _parameters.anistropic = anistropic;
    _parameters.useMaxAnistropic = maxAnistropic;
    _dirtyFlag = (TexParameterDirtyFlag)(_dirtyFlag | TexParameterDirtyFlag::TEXPARAMETER_ANISTROPIC_FILTER_MASK);
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
    ApplyAnistropicFilter();
}

void Texture::ApplyWrapParameter()
{
    bool wrap_s_mask = (_mask & TexParameterMask::TEXPARAMETER_WRAP_S_MASK) != 0;
    bool wrap_t_mask = (_mask & TexParameterMask::TEXPARAMETER_WRAP_T_MASK) != 0;
    bool wrap_r_mask = (_mask & TexParameterMask::TEXPARAMETER_WRAP_R_MASK) != 0;

    bool useSampler = _sampler != nullptr && _sampler->IsValid();
    uint32 samplerId = -1;
    if(useSampler)
        samplerId = _sampler->GetRenderRes().id;

    // wrap
    if (wrap_s_mask && 
        (_dirtyFlag & TexParameterDirtyFlag::TEXPARAMETER_WRAP_S_MASK) != 0)
    {
        if(useSampler)
            glSamplerParameteri(samplerId, GL_TEXTURE_WRAP_S, (int32)(_parameters.wrapModes[0]));
        else
            glTexParameteri(_res.type, GL_TEXTURE_WRAP_S, (int32)(_parameters.wrapModes[0]));

        _dirtyFlag = (TexParameterDirtyFlag)(
            (uint32)_dirtyFlag & (!((uint32)TexParameterDirtyFlag::TEXPARAMETER_WRAP_S_MASK))
            );
    }
    if (wrap_t_mask && 
        (_dirtyFlag & TexParameterDirtyFlag::TEXPARAMETER_WRAP_T_MASK) != 0)
    {
        if (useSampler)
            glSamplerParameteri(samplerId, GL_TEXTURE_WRAP_T, (int32)(_parameters.wrapModes[1]));
        else
            glTexParameteri(_res.type, GL_TEXTURE_WRAP_T, (int32)(_parameters.wrapModes[1]));

        _dirtyFlag = (TexParameterDirtyFlag)(
            (uint32)_dirtyFlag & (!((uint32)TexParameterDirtyFlag::TEXPARAMETER_WRAP_T_MASK))
            );
    }
    if (wrap_r_mask &&
        (_dirtyFlag & TexParameterDirtyFlag::TEXPARAMETER_WRAP_R_MASK) != 0)
    {
        if (useSampler)
            glSamplerParameteri(samplerId, GL_TEXTURE_WRAP_S, (int32)(_parameters.wrapModes[2]));
        else
            glTexParameteri(_res.type, GL_TEXTURE_WRAP_R, (int32)(_parameters.wrapModes[2]));

        _dirtyFlag = (TexParameterDirtyFlag)(
            (uint32)_dirtyFlag & (!((uint32)TexParameterDirtyFlag::TEXPARAMETER_WRAP_R_MASK))
            );
    }
}

void Texture::ApplyFilterParameter()
{
    bool filter_min_mask = (_mask & TexParameterMask::TEXPARAMETER_FILTER_MIN_MASK) != 0;
    bool filter_mag_mask = (_mask & TexParameterMask::TEXPARAMETER_FILTER_MAG_MASK) != 0;

    bool useSampler = _sampler != nullptr && _sampler->IsValid();
    uint32 samplerId = -1;
    if(useSampler)
        samplerId = _sampler->GetRenderRes().id;

    // Minification filter
    if (filter_min_mask && (_dirtyFlag & TexParameterDirtyFlag::TEXPARAMETER_FILTER_MIN_MASK) != 0)
    {
        if(useSampler)
            glSamplerParameteri(samplerId, GL_TEXTURE_MIN_FILTER, (int32)(_parameters.filterModes[0]));
        else
            glTexParameteri(_res.type, GL_TEXTURE_MIN_FILTER, (int32)(_parameters.filterModes[0]));

        _dirtyFlag = (TexParameterDirtyFlag)(
            (uint32)_dirtyFlag & (!((uint32)TexParameterDirtyFlag::TEXPARAMETER_FILTER_MIN_MASK)));
    }

    // Magnification filter
    if (filter_mag_mask && (_dirtyFlag & TexParameterDirtyFlag::TEXPARAMETER_FILTER_MAG_MASK) != 0)
    {
        if (useSampler)
            glSamplerParameteri(samplerId, GL_TEXTURE_MAG_FILTER, (int32)(_parameters.filterModes[1]));
        else
            glTexParameteri(_res.type, GL_TEXTURE_MAG_FILTER, (int32)(_parameters.filterModes[1]));

        _dirtyFlag = (TexParameterDirtyFlag)(
            (uint32)_dirtyFlag & (!((uint32)TexParameterDirtyFlag::TEXPARAMETER_FILTER_MAG_MASK)));
    }
}

void Texture::ApplySwizzleParameter()
{
    bool swizzle_mask = (_mask & TexParameterMask::TEXPARAMETER_SWIZZLE_MASK) != 0;

    // swizzle
    if (swizzle_mask && (_dirtyFlag & TexParameterDirtyFlag::TEXPARAMETER_SWIZZLE_MASK) != 0)
    {
        uint32 swizzle_param = uint32(_parameters.swizzleParameter);
        int32 index = swizzle_param - (uint32)(SwizzleParam::SWIZZLE_R);
        if (index == 4)
        {
            glTexParameteriv(_res.type, swizzle_param, reinterpret_cast<const int32 *>(_parameters.swizzle));
        }
        else
        {
            glTexParameteri(_res.type, swizzle_param, (int32)(_parameters.swizzle[index]));
        }

        _dirtyFlag = (TexParameterDirtyFlag)(
            (uint32)_dirtyFlag & (!((uint32)TexParameterDirtyFlag::TEXPARAMETER_SWIZZLE_MASK)));
    }
}

void Texture::ApplyLodBias()
{
    bool lodbias_mask = (_mask & TexParameterMask::TEXPARAMETER_LODBIAS_MASK) != 0;

    bool useSampler = _sampler != nullptr && _sampler->IsValid();
    uint32 samplerId = -1;
    if (useSampler)
        samplerId = _sampler->GetRenderRes().id;

    // lod bias
    if (lodbias_mask && _dirtyFlag & TexParameterDirtyFlag::TEXPARAMETER_LODBIAS_MASK != 0)
    {
        if(useSampler)
            glSamplerParameteri(samplerId, GL_TEXTURE_LOD_BIAS, _parameters.lodbias);
        else
            glTexParameterf(_res.type, GL_TEXTURE_LOD_BIAS, _parameters.lodbias);

        _dirtyFlag = (TexParameterDirtyFlag)(
            (uint32)_dirtyFlag & (!((uint32)TexParameterDirtyFlag::TEXPARAMETER_LODBIAS_MASK)));
    }
}

void Texture::ApplyDepthTexCompParameter()
{
    bool depth_tex_mode_mask = (_mask & TexParameterMask::TEXPARAMETER_DEPTH_TEX_COMP_MODE_MASK) != 0;
    bool depth_tex_func_mask = (_mask & TexParameterMask::TEXPARAMETER_DEPTH_TEX_COMP_FUNC_MASK) != 0;

    bool useSampler = _sampler != nullptr && _sampler->IsValid();
    uint32 samplerId = -1;
    if (useSampler)
        samplerId = _sampler->GetRenderRes().id;

    // depth tex compare mode
    if (depth_tex_mode_mask && (_dirtyFlag & TexParameterDirtyFlag::TEXPARAMETER_DEPTH_TEX_COMP_MODE_MASK) != 0)
    {
        _dirtyFlag = (TexParameterDirtyFlag)(
            (uint32)_dirtyFlag & (!((uint32)TexParameterDirtyFlag::TEXPARAMETER_DEPTH_TEX_COMP_MODE_MASK)));
    }

    // depth tex compare func
    if (depth_tex_func_mask && (_dirtyFlag & TexParameterDirtyFlag::TEXPARAMETER_DEPTH_TEX_COMP_FUNC_MASK) != 0)
    {
        _dirtyFlag = (TexParameterDirtyFlag)(
            (uint32)_dirtyFlag & (!((uint32)TexParameterDirtyFlag::TEXPARAMETER_DEPTH_TEX_COMP_FUNC_MASK)));
    }
}

void Texture::ApplyMipMapParameter()
{
    bool mipmap_base_level_mask = (_mask & TexParameterMask::TEXPARAMETER_MIPMAP_BASE_LEVEL_MASK) != 0;
    bool mipmap_max_level_mask = (_mask & TexParameterMask::TEXPARAMETER_MIPMAP_MAX_LEVEL_MASK) != 0;

    bool useSampler = _sampler != nullptr && _sampler->IsValid();
    uint32 samplerId = -1;
    if (useSampler)
        samplerId = _sampler->GetRenderRes().id;

    if (mipmap_base_level_mask && (_dirtyFlag & TexParameterDirtyFlag::TEXPARAMETER_MIPMAP_BASE_LEVEL_MASK) != 0)
    {
        glTexParameterf(_res.type, GL_TEXTURE_BASE_LEVEL, _parameters.mipmapBaselevel);

        _dirtyFlag = (TexParameterDirtyFlag)(
            (uint32)_dirtyFlag & (!((uint32)TexParameterDirtyFlag::TEXPARAMETER_MIPMAP_BASE_LEVEL_MASK)));
    }

    if (mipmap_max_level_mask && (_dirtyFlag & TexParameterDirtyFlag::TEXPARAMETER_MIPMAP_MAX_LEVEL_MASK) != 0)
    {
        glTexParameterf(_res.type, GL_TEXTURE_MAX_LEVEL, _parameters.mipmapMaxlevel);

        _dirtyFlag = (TexParameterDirtyFlag)(
            (uint32)_dirtyFlag & (!((uint32)TexParameterDirtyFlag::TEXPARAMETER_MIPMAP_MAX_LEVEL_MASK)));
    }
}

void Texture::ApplyTexLODParameter()
{
    bool tex_min_lod_mask = (_mask & TexParameterMask::TEXPARAMETER_MIN_LOD_MASK) != 0;
    bool tex_max_lod_mask = (_mask & TexParameterMask::TEXPARAMETER_MAX_LOD_MASK) != 0;

    bool useSampler = _sampler != nullptr && _sampler->IsValid();
    uint32 samplerId = -1;
    if (useSampler)
        samplerId = _sampler->GetRenderRes().id;

    if (tex_min_lod_mask && (_dirtyFlag & TexParameterDirtyFlag::TEXPARAMETER_MIN_LOD_MASK) != 0)
    {
        _dirtyFlag = (TexParameterDirtyFlag)(
            (uint32)_dirtyFlag & (!((uint32)TexParameterDirtyFlag::TEXPARAMETER_MIN_LOD_MASK)));
    }

    if (tex_max_lod_mask && (_dirtyFlag & TexParameterDirtyFlag::TEXPARAMETER_MAX_LOD_MASK) != 0)
    {
        _dirtyFlag = (TexParameterDirtyFlag)(
            (uint32)_dirtyFlag & (!((uint32)TexParameterDirtyFlag::TEXPARAMETER_MAX_LOD_MASK)));
    }
}

void Texture::ApplyTexBorderColor()
{
    bool border_color_mask = (_mask & TexParameterMask::TEXPARAMETER_BORDERCOLOR_MASK) != 0;

    bool useSampler = _sampler != nullptr && _sampler->IsValid();
    uint32 samplerId = -1;
    if (useSampler)
        samplerId = _sampler->GetRenderRes().id;

    if (border_color_mask && (_dirtyFlag & TexParameterDirtyFlag::TEXPARAMETER_BORDERCOLOR_MASK) != 0)
    {
        if(useSampler)
            glSamplerParameterfv(samplerId, GL_TEXTURE_BORDER_COLOR
                        , glm::value_ptr<float32>(_parameters.borderColor));
        else
            glTexParameterfv(_res.type, (uint32)TextureBorderColorParam::BORDER_COLOR
                        , glm::value_ptr<float32>(_parameters.borderColor));

        _dirtyFlag = (TexParameterDirtyFlag)(
            (uint32)_dirtyFlag & (!((uint32)TexParameterDirtyFlag::TEXPARAMETER_BORDERCOLOR_MASK)));
    }
}

void Texture::ApplyDepthStencilMode()
{
    bool depthstencil_mask = (_mask & TexParameterMask::TEXPARAMETER_DEPTHSTENCIL_MASK) != 0;

    bool useSampler = _sampler != nullptr && _sampler->IsValid();
    uint32 samplerId = -1;
    if (useSampler)
        samplerId = _sampler->GetRenderRes().id;

    if (depthstencil_mask && (_dirtyFlag & TexParameterDirtyFlag::TEXPARAMETER_DEPTHSTENCIL_MASK) != 0)
    {

        _dirtyFlag = (TexParameterDirtyFlag)(
            (uint32)_dirtyFlag & (!((uint32)TexParameterDirtyFlag::TEXPARAMETER_DEPTHSTENCIL_MASK)));
    }
}

void Texture::ApplyAnistropicFilter()
{
    bool mask = (_mask & TexParameterMask::TEXPARAMETER_ANISTROPIC_FILTER_MASK) != 0;

    if(mask && (_dirtyFlag & TexParameterDirtyFlag::TEXPARAMETER_ANISTROPIC_FILTER_MASK) != 0)
    {
        float32 anistropic;
        float32 maxAnistropic;
        glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &maxAnistropic);

        if(_parameters.useMaxAnistropic)
        {
            anistropic = maxAnistropic;
        }
        else if(_parameters.anistropic > maxAnistropic)
        {
            anistropic = maxAnistropic;
        }
        else
        {
            anistropic = _parameters.anistropic;             
        }
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, anistropic);

        _dirtyFlag = (TexParameterDirtyFlag)(
            (uint32)_dirtyFlag & (!((uint32)TexParameterDirtyFlag::TEXPARAMETER_ANISTROPIC_FILTER_MASK)));
    }
}

/*------------------------------Texture1D----------------------------- */


void Texture1D::InitStorage()
{
    Texture::InitStorage();

    ImageData* srcimage = nullptr;
    if(_image != nullptr)
    {
        const ImageData& source = _image->GetImageSource();
        srcimage = new ImageData(source);
    }
    
    if(srcimage != nullptr)
    {
        if(_immutable)
        {
            glTexStorage1D(_res.type,
                        srcimage->mipLevels,
                        srcimage->internalFormat,
                        srcimage->mip[0].width);

            for (int32 level = 0, mipLevels = srcimage->mipLevels; level < mipLevels; ++level)
            {
                glTexSubImage1D(_res.type,
                                level,
                                0,
                                srcimage->mip[level].width,
                                srcimage->format, 
                                srcimage->type,
                                srcimage->mip[level].data);
            }
        }
        else
        {
            for (int32 level = 0, mipLevels = srcimage->mipLevels; level < mipLevels; ++level)
            {
                glTexImage1D(_res.type,
                            level,
                            srcimage->internalFormat,
                            srcimage->mip[level].width,
                            0,
                            srcimage->format,
                            srcimage->type,
                            srcimage->mip[level].data);
            }
        }

        if(_generateMipMap)
        {
            glGenerateMipmap(_res.type);
        }

        SAFE_DEL(srcimage);
    }
    else
    {
        glTexStorage1D(_res.type,
                       _miplevels,
                       _internalformat,
                       _width);
    }

    glBindTexture(_res.type, 0);
}



/*------------------------------Texture2D----------------------------- */


void Texture2D::InitStorage()
{
    Texture::InitStorage();

    ImageData* srcimage = nullptr;
    if(_image != nullptr)
    {
        const ImageData& source = _image->GetImageSource();
        srcimage = new ImageData(source);
    }
    
    if(srcimage != nullptr)
    {
        const ImageData& image_source = _image->GetImageSource();
        if(_immutable)
        {
            glTexStorage2D(_res.type, srcimage->mipLevels, 
                            srcimage->internalFormat, 
                            srcimage->mip[0].width, 
                            srcimage->mip[0].height
            );

            for (int32 level = 0, mipLevels = srcimage->mipLevels; level < mipLevels; ++level)
            {
                glTexSubImage2D(_res.type,
                                level,
                                0, 0,
                                srcimage->mip[level].width,
                                srcimage->mip[level].height,
                                srcimage->format,
                                srcimage->type,
                                srcimage->mip[level].data);
            }
        }
        else
        {
            for (int32 level = 0, mipLevels = srcimage->mipLevels; level < mipLevels; ++level)
            {
                glTexImage2D(_res.type,
                            level,
                            srcimage->internalFormat,
                            srcimage->mip[level].width,
                            srcimage->mip[level].height,
                            0,
                            srcimage->format,
                            srcimage->type,
                            srcimage->mip[level].data);
            }
        }

        if(_generateMipMap)
        {
            glGenerateMipmap(_res.type);
        }

        SAFE_DEL(srcimage);
    }
    else
    {
        // glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, _width, _height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);

        glTexStorage2D(_res.type, _miplevels,
                       _internalformat,
                       _width,
                       _height);
    }
    glBindTexture(_res.type, 0);
}


void Texture2DMultiSample::InitStorage()
{
    Texture::InitStorage();

    assert(_width > 0 && _height > 0);

    // glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, 4, GL_RGB, _width, _height, GL_TRUE);

    glTexStorage2DMultisample(_res.type, _samples, _internalformat, _width, _height,
        _fixedsampledlocation);

    glBindTexture(_res.type, 0);   
}



void Texture3D::InitStorage()
{
    Texture::InitStorage();

    ImageData* srcimage = nullptr;
    if(_image != nullptr)
    {
        const ImageData& source = _image->GetImageSource();
        srcimage = new ImageData(source);
    }
    
    if(srcimage != nullptr)
    {
        const ImageData& image_source = _image->GetImageSource();
        if(_immutable)
        {
            glTexStorage3D(_res.type, srcimage->mipLevels, 
                            srcimage->internalFormat, 
                            srcimage->mip[0].width, 
                            srcimage->mip[0].height,
                            srcimage->mip[0].depth
            );

            for (int32 level = 0, mipLevels = srcimage->mipLevels; level < mipLevels; ++level)
            {
                glTexSubImage3D(_res.type,
                                level,
                                0, 0, 0,
                                srcimage->mip[level].width,
                                srcimage->mip[level].height,
                                srcimage->mip[level].depth,
                                srcimage->format,
                                srcimage->type,
                                srcimage->mip[level].data);
            }
        }
        else
        {
            for (int32 level = 0, mipLevels = srcimage->mipLevels; level < mipLevels; ++level)
            {
                glTexImage3D(_res.type,
                            level,
                            srcimage->internalFormat,
                            srcimage->mip[level].width,
                            srcimage->mip[level].height,
                            srcimage->mip[level].depth,
                            0,
                            srcimage->format,
                            srcimage->type,
                            srcimage->mip[level].data);
            }
        }

        if (_generateMipMap)
        {
            glGenerateMipmap(_res.type);
        }

        SAFE_DEL(srcimage);
    }

    glBindTexture(_res.type, 0);
}

void TextureRectangle::InitStorage()
{
    Texture::InitStorage();


    ImageData* srcimage = nullptr;
    if(_image != nullptr)
    {
        const ImageData& source = _image->GetImageSource();
        srcimage = new ImageData(source);
    }
    
    if(srcimage != nullptr)
    {
        glTexImage2D(_res.type,
                     0,
                     srcimage->internalFormat,
                     srcimage->mip[0].width,
                     srcimage->mip[0].height,
                     0,
                     srcimage->format,
                     srcimage->type,
                     srcimage->mip[0].data);

        SAFE_DEL(srcimage);
    }

    glBindTexture(_res.type, 0);    
}

void TextureBuffer::InitStorage()
{
    Texture::InitStorage();

    glGenBuffers(1, &_buffer);

    glBindBuffer(_res.type, _buffer);
    glBufferData(_res.type, _size, _bufferData, GL_STATIC_DRAW);

    glTexBuffer(_res.type, _internalformat, _buffer);

    glBindBuffer(_res.type, 0);
    glBindTexture(_res.type, 0);
}

TextureBuffer::~TextureBuffer()
{
    glBindBuffer(_res.type, 0);
    glBindTexture(_res.type, 0);

    glDeleteBuffers(1, &_buffer);

    if(_bufferData != nullptr)
    {

    }
}


void TextureCube::SetPositiveX(Image::Ptr image)
{
    _imagePositiveX = image;
}
void TextureCube::SetPositiveY(Image::Ptr image)
{
    _imagePositiveY = image;
}
void TextureCube::SetPositiveZ(Image::Ptr image)
{
    _imagePositiveZ = image;
}

void TextureCube::SetNegativeX(Image::Ptr image)
{
    _imageNegativeX = image;
}
void TextureCube::SetNegativeY(Image::Ptr image)
{
    _imageNegativeY = image;
}
void TextureCube::SetNegativeZ(Image::Ptr image)
{
    _imageNegativeZ = image;
}

void TextureCube::InitStorage()
{
    Texture::InitStorage();

    bool useOthers = false;

    ImageData* srcimage = nullptr;
    if(_image != nullptr)
    {
        const ImageData& source = _image->GetImageSource();
        srcimage = new ImageData(source);
    }
    else
    {
        useOthers = (_imagePositiveX != nullptr) 
                        && (_imagePositiveY != nullptr)
                        && (_imagePositiveZ != nullptr)
                        && (_imageNegativeX != nullptr)
                        && (_imageNegativeY != nullptr)
                        && (_imageNegativeZ != nullptr);
    }

    // if (srcimage != nullptr)
    {
        // const ImageData& image_source = _image->GetImageSource();
        if(_immutable)
        {
            if(!useOthers && srcimage != nullptr)
            {
                InitTexStorage(srcimage);
            }
            else if(useOthers)
            {
                ImageData* data = _imagePositiveX->GetImageSourcePtr();
                glTexStorage2D(_res.type, data->mipLevels,
                               data->internalFormat,
                               data->mip[0].width,
                               data->mip[0].height);

                InitTexStorage(GL_TEXTURE_CUBE_MAP_POSITIVE_X, _imagePositiveX->GetImageSourcePtr());
                InitTexStorage(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, _imageNegativeX->GetImageSourcePtr());
                InitTexStorage(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, _imagePositiveY->GetImageSourcePtr());
                InitTexStorage(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, _imageNegativeY->GetImageSourcePtr());
                InitTexStorage(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, _imagePositiveZ->GetImageSourcePtr());
                InitTexStorage(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, _imageNegativeZ->GetImageSourcePtr());
            }
        }
        else
        {
            if(!useOthers && srcimage != nullptr)
            {
                GLubyte* ptr = nullptr;
                for (int32 level = 0, mipLevels = srcimage->mipLevels; level < mipLevels; ++level)
                {
                    ptr = (GLubyte *)srcimage->mip[level].data;
                    for (int32 face = 0; face < 6; face++)
                    {
                        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + face,
                                     level,
                                     srcimage->internalFormat,
                                     srcimage->mip[level].width, srcimage->mip[level].height,
                                     0,
                                     srcimage->format, srcimage->type,
                                     ptr + srcimage->sliceStride * face);
                    }
                }
            }
            else if(useOthers)
            {
                ImageData * datas[6] = {_imagePositiveX->GetImageSourcePtr(),
                                   _imageNegativeX->GetImageSourcePtr(),
                                   _imagePositiveY->GetImageSourcePtr(),
                                   _imageNegativeY->GetImageSourcePtr(),
                                   _imagePositiveZ->GetImageSourcePtr(),
                                   _imageNegativeZ->GetImageSourcePtr() };

                for (int32 face = 0; face < 6; face++)
                {
                    for (int32 level = 0, mipLevels = datas[face]->mipLevels; level < mipLevels; ++level)
                    {
                        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + face,
                                     level,
                                     datas[face]->internalFormat,
                                     datas[face]->mip[level].width, datas[face]->mip[level].height,
                                     0,
                                     datas[face]->format, datas[face]->type,
                                     datas[face]->mip[level].data);
                    }
                }
            }
        }

        if (_generateMipMap)
        {
            glGenerateMipmap(_res.type);
        }

        SAFE_DEL(srcimage);
    }    

    glBindTexture(_res.type, 0);
}

void TextureCube::InitTexStorage(int32 target, ImageData *data)
{
    for (int32 level = 0, mipLevels = data->mipLevels; level < mipLevels; ++level)
    {
        glTexSubImage2D(target,
                        level,
                        0, 0,
                        data->mip[level].width,
                        data->mip[level].height,
                        data->format,
                        data->type,
                        data->mip[level].data);
    }
}

void TextureCube::InitTexStorage(ImageData *data)
{
    glTexStorage2D(_res.type, data->mipLevels,
                   data->internalFormat,
                   data->mip[0].width,
                   data->mip[0].height);

    GLubyte *ptr = nullptr;
    for (int32 level = 0, mipLevels = data->mipLevels; level < mipLevels; ++level)
    {
        ptr = (GLubyte *)data->mip[level].data;
        for (int face = 0; face < 6; face++)
        {
            glTexSubImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + face,
                            level,
                            0, 0,
                            data->mip[level].width,
                            data->mip[level].height,
                            data->format,
                            data->type,
                            ptr + data->sliceStride * face);
        }
    }
}

void Texture1DArray::InitStorage()
{
    Texture::InitStorage();

    ImageData* srcimage = nullptr;
    if(_image != nullptr)
    {
        const ImageData& source = _image->GetImageSource();
        srcimage = new ImageData(source);
    }

    if(srcimage != nullptr)
    {
        if(_immutable)
        {
            glTexStorage2D(_res.type,
                        srcimage->mipLevels,
                        srcimage->internalFormat,
                        srcimage->mip[0].width,
                        srcimage->slices);
            for (int level = 0; level < srcimage->mipLevels; ++level)
            {
                glTexSubImage2D(_res.type,
                                level,
                                0, 0,
                                srcimage->mip[level].width, srcimage->slices,
                                srcimage->format, srcimage->type,
                                srcimage->mip[level].data);
            }
        }
        else
        {
            for (int32 level = 0, mipLevels = srcimage->mipLevels; level < mipLevels; ++level)
            {
                glTexImage2D(_res.type,
                            level,
                            srcimage->internalFormat,
                            srcimage->mip[level].width,
                            srcimage->slices,
                            0,
                            srcimage->format,
                            srcimage->type,
                            srcimage->mip[level].data);
            }
        }

        if (_generateMipMap)
        {
            glGenerateMipmap(_res.type);
        }

        SAFE_DEL(srcimage);
    }

    glBindTexture(_res.type, 0);
}

void Texture2DArray::InitStorage()
{
    Texture::InitStorage();

    ImageData* srcimage = nullptr;
    if(_image != nullptr)
    {
        const ImageData& source = _image->GetImageSource();
        srcimage = new ImageData(source);
    }
    
    if(srcimage != nullptr)
    {
        const ImageData& image_source = _image->GetImageSource();
        if(_immutable)
        {
            glTexStorage3D(_res.type, srcimage->mipLevels, 
                            srcimage->internalFormat, 
                            srcimage->mip[0].width, 
                            srcimage->mip[0].height,
                            srcimage->slices
            );

            for (int32 level = 0, mipLevels = srcimage->mipLevels; level < mipLevels; ++level)
            {
                glTexSubImage3D(_res.type,
                                level,
                                0, 0, 0,
                                srcimage->mip[level].width,
                                srcimage->mip[level].height,
                                srcimage->slices,
                                srcimage->format,
                                srcimage->type,
                                srcimage->mip[level].data);
            }
        }
        else
        {
            for (int32 level = 0, mipLevels = srcimage->mipLevels; level < mipLevels; ++level)
            {
                glTexImage3D(_res.type,
                            level,
                            srcimage->internalFormat,
                            srcimage->mip[level].width,
                            srcimage->mip[level].height,
                            srcimage->slices,
                            0,
                            srcimage->format,
                            srcimage->type,
                            srcimage->mip[level].data);
            }
        }

        if(_generateMipMap)
        {
            glGenerateMipmap(_res.type);
        }

        SAFE_DEL(srcimage);
    }

    glBindTexture(_res.type, 0);
}


void TextureCubeArray::InitStorage()
{
    Texture::InitStorage();

    ImageData* srcimage = nullptr;
    if(_image != nullptr)
    {
        const ImageData& source = _image->GetImageSource();
        srcimage = new ImageData(source);
    }
    
    if(srcimage != nullptr)
    {
        const ImageData &image_source = _image->GetImageSource();
        glTexStorage3D(_res.type, srcimage->mipLevels,
                       srcimage->internalFormat,
                       srcimage->mip[0].width,
                       srcimage->mip[0].height,
                       srcimage->slices);

        for (int32 level = 0, mipLevels = srcimage->mipLevels; level < mipLevels; ++level)
        {
            glTexSubImage3D(_res.type,
                            level,
                            0, 0, 0,
                            srcimage->mip[level].width,
                            srcimage->mip[level].height,
                            srcimage->slices,
                            srcimage->format,
                            srcimage->type,
                            srcimage->mip[level].data);
        }
        //glTexImage3D does not support GL_TEXTURE_CUBE_MAP_ARRAY

        if(_generateMipMap)
        {
            glGenerateMipmap(_res.type);
        }

        SAFE_DEL(srcimage);
    }

    glBindTexture(_res.type, 0);    
}

void Texture2DMultiSampleArray::InitStorage()
{
    Texture::InitStorage();

    assert(_width > 0 && _height > 0);

    glTexStorage3DMultisample(_res.type, _samples, _internalformat, _width, _height
        , _depth, _fixedsampledlocation);

    //glTexStorage3DMultisample glTexImage3DMultisample
}

} // namespace TwinkleGraphics
