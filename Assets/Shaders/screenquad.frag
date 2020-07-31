#version 330 core

in vec2 texcoord;

out vec4 fragcolor;

uniform sampler2D mainTex;

void main()
{
    // fragcolor = vec4(1.0, 0.0f, 0.0f, 1.0f);
    fragcolor = texture(mainTex, texcoord);
}