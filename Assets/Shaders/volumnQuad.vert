#version 330 core
#extension GL_ARB_shading_language_include : require

#include </GLSLIncludes/common.glsl>
#include </GLSLIncludes/vertexLayout.glsl>

// layout (location = 0) in vec3 vertexPos;
// layout (location = 1) in vec4 vertexUV;

out vec3 texcoord;

uniform vec2 mainTexTiling;
uniform vec2 mainTexOffset;

uniform mat4 mvp; 
uniform mat4 rotmat;
uniform bvec2 flip;

void main()
{
    gl_Position = mvp * vec4(vertexPos.xy, 0.0f, 1.0f);
    vec2 tempTexcoord = FLIP_TEX(vertexUV.xy, flip);
    tempTexcoord = TRANSFORM_TEX(tempTexcoord, mainTex);

    texcoord = (rotmat * vec4(tempTexcoord, 1.0f, 1.0f)).stp;        //s\t\p(r)\q
}