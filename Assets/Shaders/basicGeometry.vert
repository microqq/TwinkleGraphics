#version 330 core
#extension GL_ARB_shading_language_include : require

#include </GLSLIncludes/vertexLayout.glsl>

uniform mat4 mvp;

void main()
{
    gl_Position= mvp * vec4(vertexPos , 1.f);
}