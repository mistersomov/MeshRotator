#version 320 es

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNorm;
layout(location = 2) in vec2 aTex;
layout(location = 3) in vec3 aTangent;
layout(location = 4) in ivec4 aBoneIds;
layout(location = 5) in vec4 aBoneWeights;
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

const int MAX_BONES = 100;
uniform mat4 BONES[MAX_BONES];

void main() {
    mat4 boneTransform = BONES[aBoneIds[0]] * aBoneWeights[0];
    boneTransform += BONES[aBoneIds[1]] * aBoneWeights[1];
    boneTransform += BONES[aBoneIds[2]] * aBoneWeights[2];
    boneTransform += BONES[aBoneIds[3]] * aBoneWeights[3];

    mat3 transposedModelMat = transpose(inverse(mat3(boneTransform)));
    vec3 T = normalize(transposedModelMat * aTangent);
    vec3 B = normalize(transposedModelMat * aBitangent);;
    vec3 N = normalize(transposedModelMat * aNorm);
    mat3 TBN = transpose(mat3(T, B, N));

    gl_Position = PROJECTION * VIEW * MODEL * boneTransform * vec4(aPos, 1.0);
    vs_out.fragPos = vec3(MODEL * boneTransform * vec4(aPos, 1.0));
    vs_out.texCoords = aTex;
    vs_out.tangentLightPos = TBN * lightPos;
    vs_out.tangentViewPos  = TBN * viewPos;
    vs_out.tangentFragPos  = TBN * vs_out.fragPos;
}