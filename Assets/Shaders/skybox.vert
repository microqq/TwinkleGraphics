#version 330 core
#extension GL_ARB_shading_language_include : require

#include </GLSLIncludes/vertexLayout.glsl>

out vec3 texcoord;

uniform mat4 mvp;

void main() {
  texcoord = vertexPos;
  gl_Position = (mvp * vec4(vertexPos, 1.0f)).xyww;
}