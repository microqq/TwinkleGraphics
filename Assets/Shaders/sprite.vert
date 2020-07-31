#version 330 core
#extension GL_ARB_shading_language_include : require

#include </GLSLIncludes/common.glsl>
#include </GLSLIncludes/vertexLayout.glsl>

// layout (location = 0) in vec3 vertexPos;
// layout (location = 1) in vec4 vertexUV;

out vec2 texcoord;

uniform vec2 mainTexTiling;
uniform vec2 mainTexOffset;

uniform mat4 mvp; 
uniform bvec2 flip;

void main()
{
    gl_Position = mvp * vec4(vertexPos, 1.0f);

    vec2 tempTexcoord = FLIP_TEX(vertexUV.xy, flip);
    texcoord = TRANSFORM_TEX(tempTexcoord, mainTex);
}