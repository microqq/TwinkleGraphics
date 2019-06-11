#version 330 core

layout (lines_adjacency) in;
layout (triangle_strip, max_vertices = 4) out;

void main()
{
    vec4 p0 = gl_in[0].gl_Position;
    vec4 p1 = gl_in[1].gl_Position; 
    vec4 p2 = gl_in[2].gl_Position; 
    vec4 p3 = gl_in[3].gl_Position; 

    

    vec4 p = gl_in[0].gl_Position;

    gl_Position = p;
    EmitVertex();

    gl_Position = p;
    EmitVertex();

    gl_Position = p;
    EmitVertex();

    gl_Position = p;
    EmitVertex();

    EndPrimitive();
}