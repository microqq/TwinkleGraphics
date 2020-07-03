#version 330 core

layout (location = 0) in vec3 position;

out vec3 texcoord;

uniform mat4 mvp; 

void main()
{
    vec3 v = normalize(position);
    texcoord = v;
    gl_Position = mvp * vec4(position, 1.0f);
}