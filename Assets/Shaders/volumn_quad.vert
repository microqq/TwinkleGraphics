#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec4 uv;

out vec3 texcoord;

uniform vec2 main_tex_tiling;
uniform vec2 main_tex_offset;

uniform mat4 mvp; 
uniform mat4 rotmat;
uniform bvec2 flip;

vec2 transform_texcoord(vec2 coord)
{
    vec2 ret_uv;
    vec2 tcoord = vec2(flip.x ? 1.0f - coord.x : coord.x
        , flip.y ? 1.0f - coord.y : coord.y); 

    ret_uv = tcoord * main_tex_tiling;
    ret_uv += main_tex_offset;

    return ret_uv;
}

void main()
{
    gl_Position = mvp * vec4(position.xy, 0.0f, 1.0f);
    vec2 tcoord = transform_texcoord(uv.xy);
    texcoord = (rotmat * vec4(tcoord, 1.0f, 1.0f)).stp;        //s\t\p(r)\q
}