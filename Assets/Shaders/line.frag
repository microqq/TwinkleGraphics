
#version 330 core

layout (location = 0) out vec4 fragcolor;

//中心直线
flat in vec2 l_start;
flat in vec2 l_end;

uniform mat4 mvp;
uniform vec4 line_params;
uniform vec4 viewport_params;

void main()
{
    float thickness = line_params.x * 0.25f;
    float feather = line_params.y;

    //计算片段（0,1）范围屏幕坐标
    vec2 fragcoord = (gl_FragCoord.xy - 0.5f) / viewport_params.xy;
    fragcoord.x *= viewport_params.z;

    //抗锯齿：计算片段与中心直线的距离，以此作为alpha参数blending
    vec2 v0 = l_end - l_start;
    vec2 v1 = fragcoord - l_start;
    //计算 v1 垂直于 v0 的垂直向量， v1_perp(v0) = v1 - v1_proj(v0)
    vec2 v1_perp = v1 - (dot(v1, v0) / dot(v0, v0)) * v0;
    float d = length(v1_perp);

    //(thickness - feather, thickness) smooth hermit interpolation between 0 and 1
    float a = 1.0f - smoothstep(thickness - feather, thickness, d);

    fragcolor = vec4(1.0f, 1.0f, 1.0f, a);
}