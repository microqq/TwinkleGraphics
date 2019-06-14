#version 330 core

layout (lines_adjacency) in;
layout (triangle_strip, max_vertices = 4) out;

uniform mat4 mvp;
uniform vec4 line_params;   //x:thickness, y:feather, z:miterlimit, w:aspect

vec4 generate_vertex(vec4 p1, vec4 p2, vec4 p, float orientation)
{
    float var_line_width = 0.0f;

    float thickness = line_params.x;
    float feather = line_params.y;
    float miterlimit = line_params.z;
    float aspect = line_params.w;

    vec4 p_cvv = mvp * p;
    vec2 p_screen = (p_cvv.xy / p_cvv.w) * vec2(aspect, 1.0f);

    vec4 p1_cvv = mvp * p1;
    p1_cvv.xyz /= p1_cvv.w;
    vec2 p1_screen = p1_cvv.xy * vec2(aspect, 1.0f);

    vec4 p2_cvv = mvp * p2;
    p2_cvv.xyz /= p2_cvv.w;
    vec2 p2_screen = p2_cvv.xy * vec2(aspect, 1.0f);

    vec2 tangent1 = p_screen - p1_screen;
    vec2 tangent2 = p2_screen - p_screen;

    vec2 n1 = normalize(vec2(-tangent1.y, tangent1.x));
    vec2 n2 = normalize(vec2(-tangent2.y, tangent2.x));

    float line_width = thickness * var_line_width / p_cvv.w + (1.0f - var_line_width) * thickness;

    vec4 result = p_cvv;
    vec2 miter;
    if(p == p1)
    {
        miter = p_screen + n2 * orientation * line_width;
    }
    else if(p2 == p)
    {
        miter = p_screen + n1 * orientation * line_width;
    }
    else
    {
        float k1 = tangent1.y / tangent1.x;
        float k2 = tangent2.y / tangent2.x;

        //line1:
        vec2 l1_start = p1_screen + n1 * orientation * line_width;

        //line2:
        vec2 l2_start = p2_screen + n2 * orientation * line_width;

        miter.x = (k1 * l1_start.x - k2 * l2_start.x + l2_start.y - l1_start.y) / (k1 - k2);
        miter.y = k1 * (miter.x - l1_start.x) + l1_start.y;
    }

    miter.x /= aspect;
    miter *= p_cvv.w;

    return vec4(miter.xy, p_cvv.zw);
}

void main()
{
    vec4 p0 = gl_in[0].gl_Position;
    vec4 p1 = gl_in[1].gl_Position;
    vec4 p2 = gl_in[2].gl_Position;
    vec4 p3 = gl_in[3].gl_Position;

    gl_Position = generate_vertex(p0, p2, p1, 1.0f);
    EmitVertex();

    gl_Position = generate_vertex(p1, p3, p2, 1.0f);
    EmitVertex();

    gl_Position = generate_vertex(p0, p2, p1, -1.0f);
    EmitVertex();

    gl_Position = generate_vertex(p1, p3, p2, -1.0f);
    EmitVertex();

    EndPrimitive();
}