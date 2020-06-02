#version 330 core

layout (location = 0) in vec3 in_position;
layout (location = 1) in vec4 in_texcoord;

out vec2 texcoord;

void main()
{
    texcoord = in_texcoord.xy;
    gl_Position = vec4(in_position, 1.0f);
}