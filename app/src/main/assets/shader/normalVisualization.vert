#version 320 es

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNorm;
layout(std140) uniform Matrices {
    mat4 PROJECTION;
};

out VS_OUT {
    vec3 normal;
} vs_out;

uniform mat4 MODEL_VIEW;

void main() {
    gl_Position = MODEL_VIEW * vec4(aPos, 1.0);
    mat3 normalMatrix = mat3(transpose(inverse(MODEL_VIEW)));
    vs_out.normal = normalize(vec3(vec4(normalMatrix * aNorm, 0.0)));
}