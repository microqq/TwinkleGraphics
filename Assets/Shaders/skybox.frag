#version 330 core

in vec3 texcoord;

out vec4 fragcolor;

uniform samplerCube main_tex;
uniform vec4 tint_color;

void main()
{
    fragcolor = texture(main_tex, texcoord) * tint_color;
}