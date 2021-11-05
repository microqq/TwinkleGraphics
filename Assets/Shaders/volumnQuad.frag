#version 330 core

in vec3 texcoord;

out vec4 fragcolor;

uniform sampler3D mainTex;
uniform vec4 tintColor;

void main() {
  vec4 tex_color = texture(mainTex, texcoord);
  fragcolor = tex_color * tintColor;
}