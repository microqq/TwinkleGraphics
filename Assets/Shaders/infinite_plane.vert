#version 330 core

layout (location = 0) in vec3 position;

//ray start point
out vec3 near_position;
//ray end point
out vec3 far_position;

uniform mat4 mvp;

void main()
{
    mat4 mvp_inv = inverse(mvp);
    vec4 inv_position = (mvp_inv * vec4(position.xy, -1.0f, 1.0f));
    near_position = inv_position.xyz / inv_position.w;

    inv_position = (mvp_inv * vec4(position.xy, 1.0f, 1.0f));
    far_position = inv_position.xyz / inv_position.w;

    gl_Position = vec4(position.xy, -1.0f, 1.0f);
}