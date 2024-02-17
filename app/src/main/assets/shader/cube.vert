#version 300 es

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNorm;
layout(location = 2) in vec2 aTex;

out vec3 normal;
out vec3 fragPos;
out vec2 texCoords;

layout(std140) uniform Matrices {
    mat4 PROJECTION;
};
uniform mat4 MODEL_VIEW;

void main() {
    fragPos = vec3(MODEL_VIEW * vec4(aPos, 1.0));
    normal = mat3(transpose(inverse(MODEL_VIEW))) * aNorm;
    texCoords = aTex;
    gl_Position = PROJECTION * MODEL_VIEW * vec4(aPos, 1.0);
}