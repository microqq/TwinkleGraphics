#version 330 core

layout(location=0)out vec4 fragcolor;

uniform vec4 tintColor;

void main()
{
    fragcolor = tintColor;
}