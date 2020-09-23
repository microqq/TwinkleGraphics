#version 330 core

layout(location=0)out vec4 fragcolor;

#if defined(VERTUV) && defined(DIFFTEX0)
    in vec2 inUV;
    uniform sampler2D diffuseTex0;
#endif
    uniform vec4 tintColor;

void main()
{
#if defined(VERTUV) && defined(DIFFTEX0)
    vec4 diffuseColor = texture(diffuseTex0, inUV);
    fragcolor = diffuseColor * tintColor;
#else
    fragcolor = tintColor;
#endif
}