#version 330 core
#extension GL_ARB_conservative_depth : enable

layout (location = 0) out vec4 fragcolor;
layout (depth_greater) out float gl_FragDepth;

//line parametric equation: P0 + Vt, eg: P0 = near_position V = far_position - near_position
in vec3 near_position;
in vec3 far_position;

//plane equation: Ax + By + Cz + D = 0
//the line intersect plane at P1, compute parameter t = (-D - (A, B, C) * P0) / (A, B, C) * V
uniform vec4 plane_param;
uniform mat4 mvp;
uniform mat3 rotate_mat;

uniform sampler2D main_tex;

/**
Martin: https://github.com/martin-pr/possumwood/wiki/Infinite-ground-plane-using-GLSL-shaders
*/
float checkerboard(vec2 R, float scale) {
	return float((
		int(floor(R.x / scale)) +
		int(floor(R.y / scale))
	) % 2);
}

void main()
{
	vec3 plane_normal = plane_param.xyz;
	vec3 ray_direction = far_position - near_position;

	float dot_PR = dot(plane_normal, ray_direction);
	if(abs(dot_PR) < 0.00001f)
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

	// vec3 pos = intersect - plane_normal * (-plane_param.w);
	// pos = rotate_mat * pos;
	// vec2 texcoord = (pos.xz) / 16.0f;
	// fragcolor = texture(main_tex, texcoord) * vec4(0.0f, 0.5f, 2.0f, 1.0f);	

	/**
	Martin: https://github.com/martin-pr/possumwood/wiki/Infinite-ground-plane-using-GLSL-shaders
	*/
	// float c =
	// 	checkerboard(intersect.xz, 1.0f) * 0.3f +
	// 	checkerboard(intersect.xz, 10.0f) * 0.2f +
	// 	checkerboard(intersect.xz, 100.0f) * 0.1f +
	// 	0.1f;
	// fragcolor = vec4(vec3(0.0f, 0.0f, c * 0.5f + 0.3f), 1f);

	// https://zhuanlan.zhihu.com/p/66637363
	// http://madebyevan.com/shaders/grid/
	vec2 grid = abs(fract(intersect.xz - 1.0f) - 0.5f) / fwidth(intersect.xz);
	float line = min(grid.x, grid.y);

	fragcolor = vec4(vec3(1.0f - min(line, 1.0f)), 1.0f);
}