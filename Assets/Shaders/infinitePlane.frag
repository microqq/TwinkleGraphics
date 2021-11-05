#version 330 core
#extension GL_ARB_conservative_depth : enable

layout(location = 0) out vec4 fragcolor;
layout(depth_greater) out float gl_FragDepth;

// line parametric equation: P0 + Vt, eg: P0 = nearPosition V = farPosition -
// nearPosition
in vec3 nearPosition;
in vec3 farPosition;

// plane equation: Ax + By + Cz + D = 0
// the line intersect plane at P1, compute parameter t = (-D - (A, B, C) * P0) /
// (A, B, C) * V
uniform vec4 planeParam;
uniform mat4 mvp;
uniform mat3 rotateMat;

uniform sampler2D mainTex;

/**
Martin:
https://github.com/martin-pr/possumwood/wiki/Infinite-ground-plane-using-GLSL-shaders
*/
float checkerboard(vec2 R, float scale) {
  return float((int(floor(R.x / scale)) + int(floor(R.y / scale))) % 2);
}

void main() {
  vec3 planeNormal = planeParam.xyz;
  vec3 rayDirection = farPosition - nearPosition;

  float dotPR = dot(planeNormal, rayDirection);
  if (abs(dotPR) < 0.00001f) {
    discard;
  }

  float t = (-planeParam.w - dot(planeNormal, nearPosition)) / dotPR;
  if (t >= 0.99f || t < 0.0f) {
    discard;
  }
  vec3 intersect = nearPosition + t * rayDirection;
  vec4 intersectProj = mvp * vec4(intersect, 1.0f);

  float near = gl_DepthRange.near;
  float far = gl_DepthRange.far;
  float depth =
      (intersectProj.z / intersectProj.w + 1.0f) * 0.5f * (far - near) + near;
  gl_FragDepth = depth;

  vec3 pos = intersect - planeNormal * (-planeParam.w);
  pos = rotateMat * pos;
  vec2 texcoord = (pos.xz) / 32.0f;
  fragcolor = texture(mainTex, texcoord) * vec4(0.7f, 0.7f, 0.7f, 1.0f);

  /**
  Martin:
  https://github.com/martin-pr/possumwood/wiki/Infinite-ground-plane-using-GLSL-shaders
  */
  // float c =
  // 	checkerboard(intersect.xz, 1.0f) * 0.3f +
  // 	checkerboard(intersect.xz, 10.0f) * 0.2f +
  // 	checkerboard(intersect.xz, 100.0f) * 0.1f +
  // 	0.1f;
  // fragcolor = vec4(vec3(c * 0.5f + 0.3f), 1f);

  // https://zhuanlan.zhihu.com/p/66637363
  // http://madebyevan.com/shaders/grid/
  // vec2 grid = abs(fract(intersect.xz - 1.0f) - 0.5f) / fwidth(intersect.xz);
  // float line = min(grid.x, grid.y);

  // fragcolor = vec4(vec3(1.0f - min(line, 1.0f)), 1.0f);
}