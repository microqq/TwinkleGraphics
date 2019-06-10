#version 330 core
#extension GL_ARB_conservative_depth : enable

layout (location = 0) out vec4 out_fragcolor;
layout (depth_greater) out float gl_FragDepth;

//line parametric equation: P0 + Vt, eg: P0 = near_position V = far_position - near_position
in vec3 near_position;
in vec3 far_position;

//plane equation: Ax + By + Cz + D = 0
uniform vec4 plane_param;
uniform mat4 mvp;

//line intersect plane at P1, compute parameter t = (-D - (A, B, C) * P0) / (A, B, C) * V

void main()
{
	vec3 plane_normal = plane_param.xyz;
	vec3 ray_direction = (far_position - near_position);

	float dot_PR = dot(plane_normal, ray_direction);
	if(abs(dot_PR) < 0.0001f)
	{
		discard;
	}

	float t = (-plane_param.w - dot(plane_normal, near_position)) / dot_PR;
	if(t >= 0.99f || t < 0.0f)
	{
		discard;
	}
	vec3 intersect = near_position + t * ray_direction;
	vec4 intersect_proj = mvp * vec4(intersect, 1.0f);
	
	float near = gl_DepthRange.near;
	float far = gl_DepthRange.far;
	float depth = (intersect_proj.z / intersect_proj.w + 1.0f) * 0.5f * (far - near) + near;
	gl_FragDepth = depth;

	vec2 grid = intersect.xz;
	float fx = 1.0f - step(0.0f, mod(abs(grid.x), 10.0f) - 5.0f);
	float fy = 1.0f - step(0.0f, mod(abs(grid.y), 10.0f) - 5.0f);

	float f = fx + fy;
	f = step(1.0f, f);

	vec3 linecolor = vec3(0.6f, 0.6f, 0.6f);
	vec3 color = vec3(0.4f, 0.4f, 0.4f);

	out_fragcolor = vec4(f * linecolor + color * (1.0f - f), 1.0f);
}