#version 330 core
#extension GL_ARB_shading_language_include : require

#include </GLSLIncludes/vertexLayout.glsl>

out vec2 texcoord;

void main() {
  texcoord = vertexUV.xy;
  gl_Position = vec4(vertexPos, 1.0f);
}