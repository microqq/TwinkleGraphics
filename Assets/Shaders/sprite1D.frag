
#version 330 core

in vec2 texcoord;

out vec4 fragcolor;

uniform sampler1D mainTex;
uniform vec4 tintColor;

void main()
{
    vec4 tex_color = texture(mainTex, texcoord.x);
    fragcolor = tex_color * tintColor;
}