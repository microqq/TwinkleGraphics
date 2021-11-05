#version 330 core
#extension GL_ARB_shading_language_include : require

#include </GLSLIncludes/vertexLayout.glsl>

void main() { gl_Position = vec4(vertexPos, 1.0f); }