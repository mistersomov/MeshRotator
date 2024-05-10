#version 320 es

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNorm;
layout(location = 2) in vec2 aTex;
layout(location = 3) in vec3 aTangent;
layout(location = 4) in vec3 aBitangent;
layout(std140) uniform Matrices {
    mat4 VIEW;
    mat4 PROJECTION;
};

out VS_OUT {
    vec3 fragPos;
    vec2 texCoords;
    vec3 tangentLightPos;
    vec3 tangentViewPos;
    vec3 tangentFragPos;
} vs_out;

uniform vec3 viewPos;
uniform vec3 lightPos;
uniform mat4 MODEL;

void main() {
    mat3 normalMatrix = transpose(inverse(mat3(MODEL)));
    vec3 T = normalize(normalMatrix * aTangent);
    vec3 N = normalize(normalMatrix * aNorm);
    vec3 B = normalize(normalMatrix * aBitangent);;
    T = normalize(T - dot(T, N) * N);
    mat3 TBN = transpose(mat3(T, B, N));

    vs_out.fragPos = vec3(MODEL * vec4(aPos, 1.0));
    vs_out.texCoords = aTex;
    vs_out.tangentLightPos = TBN * lightPos;
    vs_out.tangentViewPos = TBN * viewPos;
    vs_out.tangentFragPos = TBN * vs_out.fragPos;
    gl_Position = PROJECTION * VIEW * MODEL * vec4(aPos, 1.0);
}