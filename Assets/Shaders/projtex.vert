#version 330 core

layout (location = 0) in vec3 position;

out vec4 texcoord;

uniform mat4 mvp; 
uniform mat4 texvp;

void main()
{
    texcoord = texvp * vec4(position, 1.0f);
    gl_Position = mvp * vec4(position, 1.0f);
}