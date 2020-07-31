#version 330 core
#extension GL_ARB_shading_language_include : require

#include </GLSLIncludes/vertexLayout.glsl>

//ray start point
out vec3 nearPosition;
//ray end point
out vec3 farPosition;

uniform mat4 mvp;

void main()
{
    mat4 mvpInverse = inverse(mvp);
    vec4 invPosition = (mvpInverse * vec4(vertexPos.xy, -1.0f, 1.0f));
    nearPosition = invPosition.xyz / invPosition.w;

    invPosition = (mvpInverse * vec4(vertexPos.xy, 1.0f, 1.0f));
    farPosition = invPosition.xyz / invPosition.w;

    gl_Position = vec4(vertexPos.xy, -1.0f, 1.0f);
}