#version 330 core
#extension GL_ARB_shading_language_include : require

#include </GLSLIncludes/vertexLayout.glsl>

out vec4 texcoord;

uniform mat4 mvp;
uniform mat4 texvp;

void main() {
  texcoord = texvp * vec4(vertexPos, 1.0f);
  gl_Position = mvp * vec4(vertexPos, 1.0f);
}