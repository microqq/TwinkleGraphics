#version 330 core

in vec3 texcoord;

out vec4 fragcolor;

uniform samplerCube mainTex;
uniform vec4 tintColor;

void main() { fragcolor = texture(mainTex, texcoord) * tintColor; }