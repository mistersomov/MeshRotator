#version 320 es

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNorm;
layout(location = 2) in vec2 aTex;
layout(location = 3) in vec3 aTangent;
layout(std140) uniform Matrices {
    mat4 gView;
    mat4 gProjection;
};

out VS_OUT {
    vec3 localPos;
    vec2 texCoords;
    vec3 normal;
    vec3 tangent;
} vs_out;

uniform mat4 gModel;

void main() {
    vs_out.localPos = aPos;
    vs_out.texCoords = aTex;
    vs_out.normal = aNorm;
    vs_out.tangent = aTangent;

    gl_Position = gProjection * gView * gModel * vec4(aPos, 1.0);
}