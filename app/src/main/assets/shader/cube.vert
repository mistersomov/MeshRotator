#version 320 es

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNorm;
layout(location = 2) in vec2 aTex;
layout(location = 3) in vec2 aTang;
layout(location = 4) in vec2 aBitang;
layout(std140) uniform Matrices {
    mat4 PROJECTION;
};

out VS_OUT {
    vec3 normal;
    vec3 fragPos;
    vec2 texCoords;
} vs_out;

uniform mat4 MODEL_VIEW;

void main() {
    vs_out.fragPos = vec3(MODEL_VIEW * vec4(aPos, 1.0));
    vs_out.normal = mat3(transpose(inverse(MODEL_VIEW))) * aNorm;
    vs_out.texCoords = aTex;
    gl_Position = PROJECTION * MODEL_VIEW * vec4(aPos, 1.0);
}