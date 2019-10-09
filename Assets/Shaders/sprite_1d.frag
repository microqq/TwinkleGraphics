
#version 330 core

in vec2 texcoord;

out vec4 fragcolor;

uniform sampler1D main_tex;
uniform vec4 tint_color;

void main()
{
    vec4 tex_color = texture(main_tex, texcoord.x);
    fragcolor = tex_color * tint_color;
}