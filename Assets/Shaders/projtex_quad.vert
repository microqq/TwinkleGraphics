#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec4 uv;

out vec2 texcoord;

uniform mat4 world;
uniform mat4 mvp; 
uniform mat4 texvp;

void main()
{
    gl_Position = mvp * vec4(position, 1.0f);
}