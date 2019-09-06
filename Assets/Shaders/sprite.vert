#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec4 uv;

out vec2 texcoord;

uniform vec2 main_tex_tiling;
uniform vec2 main_tex_offset;

uniform mat4 mvp; 
uniform bool flip;

void main()
{
    gl_Position = mvp * vec4(position, 1.0f);
    texcoord = uv.xy;
}