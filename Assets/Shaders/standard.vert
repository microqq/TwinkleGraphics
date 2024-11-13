#version 430 core
#extension GL_ARB_shading_language_include : require

#include </GLSLIncludes/vertexLayout.glsl>

#ifdef VERTUV
out vec2 inUV;
#endif

uniform mat4 mvp;

void main() {
#ifdef VERTUV
  inUV = vertexUV.xy;
#endif

  gl_Position = mvp * vec4(vertexPos, 1.f);
}