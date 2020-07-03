#version 330 core

layout(location=0)out vec4 out_fragcolor;

uniform vec4 tint_color;

void main()
{
    out_fragcolor = tint_color;
}