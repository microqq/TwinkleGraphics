#version 330 core

layout (location = 0) in vec3 position;

out vec3 texcoord;

uniform mat3 view;

void main()
{

    gl_Position = vec4(position.xy, 1.0f, 1.0f);
}