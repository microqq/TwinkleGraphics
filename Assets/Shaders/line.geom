#version 330 core


layout (lines_adjacency) in;
layout (triangle_strip, max_vertices = 4) out;

//flat: 中心直线不需要插值运算
flat out vec2 lineStart;
flat out vec2 lineEnd;

uniform mat4 mvp;
uniform vec4 lineParams;   //x:thickness, y:feather, z:miterlimit, w:aspect
uniform vec4 viewportParams;

vec4 generateVertex(vec4 p1, vec4 p2, vec4 p, float orientation)
{
    //控制透视投影线的宽度是否固定（后面再将该变量提出去）
    float var_line_width = 0.0f;

    float thickness = lineParams.x;
    float feather = lineParams.y;
    float miterlimit = lineParams.z;
    float aspect = viewportParams.z;

    //计算投影后点的坐标：透视修正，并根据屏幕宽高比拉伸
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

    //计算投影后二维平面直线的法向量并归一化
    vec2 n1 = normalize(vec2(-tangent1.y, tangent1.x));
    vec2 n2 = normalize(vec2(-tangent2.y, tangent2.x));

    float line_width = thickness * var_line_width / p_cvv.w + (1.0f - var_line_width) * thickness;
    line_width *= 0.5f;

    vec2 miter;
    if(p == p1) //直线端点(cap)
    {
        miter = p_screen + n2 * orientation * line_width;
    }
    else if(p2 == p) //直线端点(cap)
    {
        miter = p_screen + n1 * orientation * line_width;
    }
    else //直线连接点(joint)
    {
        /**
          p0___________________p1
          l0________________l1/
          p2_____________p3/ /
                        / / /
                       / / /
                      / / /
                   p4/ / /p5
                      l2
        **/

        //Todo: 夹角极小时采用bevel接头

        float k1 = tangent1.y / tangent1.x;
        float k2 = tangent2.y / tangent2.x;
        float epsilon = 0.01f;
        
        if(abs(k1 - k2) < epsilon)
        {
            vec2 n = normalize(n1 + n2);
            miter = p_screen + n * orientation * line_width;
        }
        else
        {

            //计算直线 (p0, p1)或（p2, p3） 和 (p1, p5) 或（p3, p4）的交点，该交点作为直线连接点（joint）
            //line1:
            vec2 l1_start = p1_screen + n1 * orientation * line_width;

            tangent1 = normalize(tangent1);
            tangent2 = normalize(tangent2);
            float inter_angle = dot(tangent1, tangent2);
            if(inter_angle <= -miterlimit)
                orientation *= -1.0f;

            //line2:
            vec2 l2_start = p2_screen + n2 * orientation * line_width;

            miter.x = (k1 * l1_start.x - k2 * l2_start.x + l2_start.y - l1_start.y) / (k1 - k2);
            miter.y = k1 * (miter.x - l1_start.x) + l1_start.y;
        }
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

    float aspect = viewportParams.z;
    vec4 p1_cvv = mvp * p1;
    vec2 p1_screen = (p1_cvv.xy / p1_cvv.w) * 0.5f + 0.5f;
    p1_screen.x *= aspect;

    vec4 p2_cvv = mvp * p2;
    vec2 p2_screen = (p2_cvv.xy / p2_cvv.w) * 0.5f + 0.5f;
    p2_screen.x *= aspect;

    //计算中心直线
    lineStart = p1_screen;
    lineEnd = p2_screen;
    //计算直线三角化的新顶点
    gl_Position = generateVertex(p0, p2, p1, 1.0f);
    EmitVertex();

    gl_Position = generateVertex(p1, p3, p2, 1.0f);
    EmitVertex();

    gl_Position = generateVertex(p0, p2, p1, -1.0f);
    EmitVertex();

    gl_Position = generateVertex(p1, p3, p2, -1.0f);
    EmitVertex();

    EndPrimitive();
}