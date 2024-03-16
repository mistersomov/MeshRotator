#version 320 es

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNorm;
layout(location = 2) in vec2 aTex;
layout(location = 3) in vec3 aTangent;
layout(location = 4) in vec3 aBitangent;
layout(std140) uniform Matrices {
    mat4 PROJECTION;
};

out VS_OUT {
    vec3 fragPos;
    vec2 texCoords;
    mat3 TBN;
} vs_out;

uniform mat4 MODEL_VIEW;

void main() {
    vec3 T = normalize(vec3(MODEL_VIEW * vec4(aTangent, 0.0)));
    vec3 B = normalize(vec3(MODEL_VIEW * vec4(aBitangent, 0.0)));
    vec3 N = normalize(vec3(MODEL_VIEW * vec4(aNorm, 0.0)));
    vs_out.fragPos = vec3(MODEL_VIEW * vec4(aPos, 1.0));
    vs_out.texCoords = aTex;
    vs_out.TBN = transpose(mat3(T, B, N));
    gl_Position = PROJECTION * MODEL_VIEW * vec4(aPos, 1.0);
}