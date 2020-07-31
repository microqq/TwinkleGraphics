#version 330 core

in vec4 texcoord;

out vec4 fragcolor;

uniform sampler2D mainTex;
uniform vec4 tintColor;

void main()
{
    vec2 proj_texcoord = texcoord.xy / texcoord.w;
    //range to 0 - 1
    proj_texcoord = proj_texcoord * 0.5f + 0.5f;

    vec4 tex_color = texture(mainTex, proj_texcoord);
    fragcolor = tex_color * tintColor + (1.0f - tex_color.a) * tintColor;
}