#version 330 core
#extension GL_ARB_shading_language_include : require

#include </GLSLIncludes/vertexLayout.glsl>

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() {
  mat4 mvp = projection * view * model;
  gl_Position = mvp * vec4(vertexPos, 1.0f);
}