#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec4 uv;

out vec2 texcoord;

uniform vec2 main_tex_tiling;
uniform vec2 main_tex_offset;

uniform mat4 mvp; 
uniform bool flip;

vec2 transform_texcoord(vec2 coord)
{
    vec2 ret_uv;
    ret_uv = coord * main_tex_tiling;
    ret_uv += main_tex_offset;

    return ret_uv;
}

void main()
{
    gl_Position = mvp * vec4(position, 1.0f);
    texcoord = transform_texcoord(uv.xy);
}