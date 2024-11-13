#version 330 core
#extension GL_ARB_shading_language_include : require

#define BOX 0
#define TRIANGLE 1
#define CUBIC 2
#define HANHAMMING 3
#define BLACKMAN 4
#define KAISER 5
#define LANCZOS3 6
#define LANCZOS4 7
#define LANCZOS6 8

#define LANCZOS3SUPPORT 3.0f
#define LANCZOS4SUPPORT 4.0f
#define LANCZOS6SUPPORT 6.0f

#include </GLSLIncludes/shaderVariables.glsl>

// RGSS?
// https://github.com/TheRealMJP/MSAAFilter
const vec2 SubSampleOffsets[4] =
    vec2[](vec2(-0.125f, -0.375f), vec2(0.375f, -0.125f), vec2(-0.375f, 0.125f),
           vec2(0.125f, 0.375f));

in vec2 texcoord;
out vec4 fragcolor;

uniform sampler2DMS mainTex;
uniform int samplecount;
uniform int filterOption;
uniform float cubicA;

ivec2 getTexelPos(sampler2DMS tex) {
  ivec2 size = textureSize(mainTex);
  return ivec2(texcoord * size);
}

vec4 fetchTexel(sampler2DMS tex, ivec2 pos, int sampleIndex) {
  return texelFetch(tex, pos, sampleIndex);
}

float sinc(float x) {
  x *= TW_PI;

  if (x != 0.0f) {
    return (sin(x) / x);
  }

  return 0.0f;
}

vec2 filterUgly(vec2 v) { return 1.0f - step(0.5f, abs(v)); }

vec2 filterBox(vec2 v) { return filterUgly(v); }

/**
    Allen B. Tucker: 《Computer Science Handbook》
    1 − |x|,    0 <= |x| < 1
    0,          |x| >= 1
*/
vec2 filterTriangle(vec2 v) { return clamp(1.0f - abs(v), 0.0f, 1.0f); }

/**
    Allen B. Tucker: 《Computer Science Handbook》
    (a + 2)|x|^3 − (a + 3)|x|^2 + 1,    0 <= |x| < 1
    a|x|^3 − 5a|x|^2 + 8a|x| − 4a,      1 <= |x| < 2
    0,                                  |x| >= 2

    "where −3 < a < 0 is used to make h resemble the sinc function"
*/
float fitlerCubicX(float x);
vec2 fitlerCubic(vec2 v) { return vec2(fitlerCubicX(v.x), fitlerCubicX(v.y)); }

float fitlerCubicX(float x) {
  // float a = cubicA;
  float a = -2.5f;
  float v = abs(x);
  float vPowOf2 = pow(v, 2.0f);
  float vPowOf3 = pow(v, 3.0f);

  if (v < 1.0f) {
    return (a + 2.0f) * vPowOf3 - (a + 3.0f) * vPowOf2 + 1.0f;
  } else if (v < 2.0f) {
    return (a * vPowOf3) - (5.0f * a * vPowOf2) - (4.0f * a);
  }

  return v = 0.0f;
}

/**
    Allen B. Tucker: 《Computer Science Handbook》
    "Windowed sinc function"
    0.42 + 0.5 * cos(2*TW_PI*x/(N-1)) + 0.08 * cos(4*TW_PI*x/(N-1)), |x| <
   (N-1)/2 0, otherwise
*/
float blackmanWindow(float x);
vec2 filterBlackman(vec2 v) {
  return vec2(sinc(v.x), sinc(v.y)) *
         vec2(blackmanWindow(v.x), blackmanWindow(v.y));
}

float blackmanWindow(float x) {
  x = abs(x);

  if (x < samplecount / 2) {
    float t = (2.0f * TW_PI * x) / samplecount; // (N-1), N must be odd
    float t1 = 2.0f * t;

    float cost = cos(t);
    float cost1 = cos(t1);

    return 0.42f + 0.5f * cost + 0.08f * cost1;
  }

  return 0.0f;
}

/**
    Allen B. Tucker: 《Computer Science Handbook》
    "Windowed sinc function"
    a + (1 − a) cos(2*TW_PI*x/(N-1)), |x| < (N-1)/2
    0, otherwise
*/
float hanHammingWindow(float x);
vec2 filterHanHamming(vec2 v) {
  return vec2(sinc(v.x), sinc(v.y)) *
         vec2(hanHammingWindow(v.x), hanHammingWindow(v.y));
}

float hanHammingWindow(float x) {
  x = abs(x);

  if (x < samplecount) {
    return (sinc(x) *
            (0.5f + 0.5f * cos(TW_PI * x / samplecount))); // a = 0.5f;
  }

  return (0.0);
}

/**
    Allen B. Tucker: 《Computer Science Handbook》
    "Windowed sinc function"
    "where I0 is the zeroth-order Bessel function of the first kind".
    the First kind of modified bessel function.

*/
float kaiserWindow(float x);
vec2 filterKaiser(vec2 v) {
  return vec2(sinc(v.x), sinc(v.y)) *
         vec2(kaiserWindow(v.x), kaiserWindow(v.y));
}

float kaiserWindow(float x) {
  float ret = 0.0f;
  return ret;
}

/**
    Allen B. Tucker: 《Computer Science Handbook》
    "Windowed sinc function"
    sinc(TW_PI * x / N), 0 <= |x| < N
    0, otherwise
    "Larger N results in superior frequency response".
*/
float lanczosWindow(float x, float support);
vec2 filterLanczos(vec2 v, float support) {
  return vec2(sinc(v.x), sinc(v.y)) *
         vec2(lanczosWindow(v.x, support), lanczosWindow(v.y, support));
}

float lanczosWindow(float x, float support) {
  x = abs(x);
  x /= support;

  if (x < support) {
    return sinc(x);
  }

  return 0.0f;
}

vec2 filterSwitch(vec2 v) {
  vec2 ret = vec2(0.0f);
  if (filterOption == BOX) {
    ret = filterBox(v);
  } else if (filterOption == TRIANGLE) {
    ret = filterTriangle(v);
  } else if (filterOption == CUBIC) {
    ret = fitlerCubic(v);
  } else if (filterOption == HANHAMMING) {
    ret = filterHanHamming(v);
  } else if (filterOption == BLACKMAN) {
    ret = filterBlackman(v);
  } else if (filterOption == KAISER) {
    ret = filterKaiser(v);
  } else if (filterOption == LANCZOS3) {
    ret = filterLanczos(v, LANCZOS3SUPPORT);
  } else if (filterOption == LANCZOS4) {
    ret = filterLanczos(v, LANCZOS4SUPPORT);
  } else if (filterOption == LANCZOS6) {
    ret = filterLanczos(v, LANCZOS6SUPPORT);
  } else {
    ret = vec2(1.0f);
  }

  return ret;
}

void main() {
  vec2 weightXY;
  float totalWeight = 0.0f, weight = 0.0f;
  ivec2 texelPos = getTexelPos(mainTex);
  vec4 s = vec4(0.0f);
  for (int i = 0; i < samplecount; i++) {
    weightXY = filterSwitch(SubSampleOffsets[i]);
    weight = weightXY.x * weightXY.y;
    s += (fetchTexel(mainTex, texelPos, i) * weight);

    totalWeight += weight;
  }

  fragcolor = s / totalWeight;
}