
#version 330 core

layout (location = 0) out vec4 fragcolor;

//中心直线
flat in vec2 lineStart;
flat in vec2 lineEnd;

uniform mat4 mvp;
uniform vec4 lineParams;
uniform vec4 viewportParams;
uniform vec3 lineColor;

void main()
{
    float thickness = lineParams.x * 0.25f;
    float feather = lineParams.y;

    //计算片段（0,1）范围屏幕坐标
    vec2 fragcoord = (gl_FragCoord.xy - 0.5f) / viewportParams.xy;
    fragcoord.x *= viewportParams.z;

    //抗锯齿：计算片段与中心直线的距离，以此作为alpha参数blending
    vec2 v0 = lineEnd - lineStart;
    vec2 v1 = fragcoord - lineStart;
    //计算 v1 垂直于 v0 的垂直向量， v1_perp(v0) = v1 - v1_proj(v0)
    vec2 v1_perp = v1 - (dot(v1, v0) / dot(v0, v0)) * v0;
    float d = length(v1_perp);

    //(thickness - feather, thickness) smooth hermit interpolation between 0 and 1
    float a = 1.0f - smoothstep(thickness - feather, thickness, d);

    fragcolor = vec4(lineColor, a * lineParams.w + (1.0f - lineParams.w));
}