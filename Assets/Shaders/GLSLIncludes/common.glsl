#ifndef TW_COMMON_INCLUDE
#define TW_COMMON_INCLUDE

#include </GLSLIncludes/shaderVariables.glsl>

#define FLIP_TEX(texcoord, flip)                         \
    (                                                    \
        vec2(flip.x ? 1.0f - texcoord.x : texcoord.x,    \
             flip.y ? 1.0f - texcoord.y : texcoord.y)    \
    )

#define TRANSFORM_TEX(texcoord, texName) (texcoord * texName##Tiling + texName##Offset)


#endif
