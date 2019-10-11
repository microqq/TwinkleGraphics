#version 330 core

layout (location = 0) in vec3 position;

out vec3 texcoord;

uniform mat4 mvp; 
uniform float size;

void main()
{
    texcoord = position / (size * 0.5f);
    gl_Position = mvp * vec4(position, 1.0f);
}