#version 330 core

in vec2 texcoord;

out vec4 fragcolor;

uniform sampler2D main_tex;

void main()
{
    // fragcolor = vec4(1.0, 0.0f, 0.0f, 1.0f);
    fragcolor = texture(main_tex, texcoord);
}