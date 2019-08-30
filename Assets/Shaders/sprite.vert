#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec4 uv;

out vec2 texcoord;

uniform vec2 main_tex_tiling;
uniform vec2 main_tex_offset;

void main()
{
    gl_Position = vec4(position, 1.0f);
    texcoord = uv.xy;
}