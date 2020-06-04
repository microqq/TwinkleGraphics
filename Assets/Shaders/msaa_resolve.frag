#version 330 core

in vec2 texcoord;

out vec4 fragcolor;

uniform sampler2DMS main_tex;

vec4 FilterUgly(sampler2DMS tex)
{
    ivec2 size = textureSize(main_tex);
    ivec2 texelPos = ivec2(texcoord * size);
    vec4 s0 = texelFetch(main_tex, texelPos, 0);
    vec4 s1 = texelFetch(main_tex, texelPos, 1);
    vec4 s2 = texelFetch(main_tex, texelPos, 2);
    vec4 s3 = texelFetch(main_tex, texelPos, 3);
    return (s0 + s1 + s2 + s3) * 0.25f;
}


void main()
{
    fragcolor = FilterUgly(main_tex);
    // fragcolor = vec4(1.0, 0.0f, 0.0f, 1.0f);
}