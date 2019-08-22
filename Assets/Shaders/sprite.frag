#version 330 core

in vec2 texcoord;

out vec4 fragcolor;

uniform sampler2D main_tex;
uniform vec4 tint_color;

void main()
{
    vec4 tex_color = texture2D(main_tex, texcoord);
    fragcolor = tex_color * tint_color;
}