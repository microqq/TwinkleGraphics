#ifndef TW_VERTEXLAYOUT_INCLUDE
#define TW_VERTEXLAYOUT_INCLUDE

layout(location = 0) in vec3 vertexPos;
#ifdef VERTCOLOR
layout(location = 1) in vec4 vertexColor;
#endif
#ifdef VERTNORMAL
layout(location = 2) in vec3 vertexNormal;
#endif
#ifdef VERTBINORMAL
layout(location = 3) in vec3 vertexBiNormal;
#endif
#ifdef VERTUV
layout(location = 4) in vec4 vertexUV;
#endif
#ifdef VERTUV1
layout(location = 5) in vec4 vertexUV1;
#endif
#ifdef VERTUV2
layout(location = 6) in vec4 vertexUV2;
#endif
#ifdef VERTUV3
layout(location = 7) in vec4 vertexUV3;
#endif
#ifdef VERTUV4
layout(location = 8) in vec4 vertexUV4;
#endif
#ifdef VERTUV5
layout(location = 9) in vec4 vertexUV5;
#endif
#ifdef VERTUV6
layout(location = 10) in vec4 vertexUV6;
#endif
#ifdef VERTUV7
layout(location = 11) in vec4 vertexUV7;
#endif

#endif