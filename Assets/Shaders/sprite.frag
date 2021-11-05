#version 330 core

in vec2 texcoord;

out vec4 fragcolor;

uniform sampler2D mainTex;
uniform vec4 tintColor;

void main() {
  vec4 texColor = texture(mainTex, texcoord);
  fragcolor = texColor * tintColor;
}