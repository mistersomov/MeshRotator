#version 320 es

precision mediump float;

#define PI 3.1415926535897932384626433832795

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct PBRMaterial {
    float roughness;
    bool isMetal;
    vec3 color;
};

struct DirLight {
    vec3 direction;
    vec3 color;
    float ambientIntensity;
    float diffuseIntensity;
};

in VS_OUT {
    vec3 localPos;
    vec2 texCoords;
    vec3 normal;
} fs_in;

uniform bool gIsPBR;
uniform PBRMaterial gPBRMaterial;
uniform Material gMaterial;
uniform DirLight gDirLight;
uniform vec3 gCameraLocalPos;

out vec4 color;

// This is D(h) component from Cook-Torrance equation,
// where h is halfway vector between light position vector and view position vector.
// D(h) = alpha^2 / (pi * (n * h)^2 * (alpha^2 - 1) + 1)^2,
// where alpha^2 is PBR material's squared roughness.
float ggxDistribution(float nDotH) {
    float alpha2 = pow(gPBRMaterial.roughness, 2.0);
    float denominator = max(pow(nDotH, 2.0) * (alpha2 - 1.0) + 1.0, 0.000001);
    return alpha2 / (PI * pow(denominator, 2.0));
}

// This is G(dp) component from Cook-Torrance equation,
// where dp is dot poduct between normal vector and light vector
// also between normal vector and view position vector.
// G(dp) = dp / (dp * (1 - k) + 1),
// where k is PBR material's squared (roughness + 1) divided by 8.
float geomSmith(float dotProduct) {
    float k = pow(gPBRMaterial.roughness + 1.0, 2.0) / 8.0;
    float denominator = max(dotProduct * (1.0 - k) + k, 0.000001);
    return dotProduct / denominator;
}

// This is F(v, h) component from Cook-Torrance equation, which named Fresnel.
// F(v, h) = F0 + (1.0 - F0) * pow(clamp(1.0 - vDotH, 0.0, 1.0), 5.0),
// where F0 is dielectric/metal roughness of PBR material. See a table of roughness from internet.
vec3 shlickFresnel(float vDotH) {
    vec3 F0 = vec3(0.04);
    if (gPBRMaterial.isMetal) {
        F0 = gPBRMaterial.color;
    }
    return F0 + (1.0 - F0) * pow(clamp(1.0 - vDotH, 0.0, 1.0), 5.0);
}

// Cook-Torrance microfacet specular shading model is
// f(l, v) = D(h) * G(l, v, h) * F(v, h) / 4 (n * l) (n * v)
vec4 calcPBRLighting() {
    vec3 nNormalized = normalize(fs_in.normal);
    vec3 view = normalize(gCameraLocalPos - fs_in.localPos);
    vec3 halfway = normalize(view + gDirLight.direction);
    vec3 lightIntensity = gDirLight.color * gDirLight.diffuseIntensity;

    float nDotH = max(dot(nNormalized, halfway), 0.0);
    float vDotH = max(dot(view, halfway), 0.0);
    float nDotL = max(dot(nNormalized, gDirLight.direction), 0.0);
    float nDotV = max(dot(nNormalized, view), 0.0);

    vec3 kS = shlickFresnel(vDotH);
    vec3 kD = 1.0 - kS;

    vec3 specBRDFNom =
            ggxDistribution(nDotH)
                * geomSmith(nDotL)
                * geomSmith(nDotV)
                * kS;
    float specBRDFDenom = 4.0 * nDotL * nDotV;
    vec3 specularBRDF = specBRDFNom / specBRDFDenom;

    vec3 fLambert = vec3(0.0);
    if (!gPBRMaterial.isMetal) {
        fLambert = gPBRMaterial.color;
    }
    vec3 diffuseBRDF = kD * fLambert / PI;

    return vec4((diffuseBRDF + specularBRDF) * lightIntensity * nDotL, 1.0);
}

vec4 calcBlinnPhongLighting() {
    vec3 nNormalized = normalize(fs_in.normal);
    vec4 ambientColor =
            gDirLight.ambientIntensity
                * vec4(gDirLight.color, 1.0)
                * vec4(gMaterial.ambient, 1.0);

    float diffuseFactor = max(dot(gDirLight.direction, nNormalized), 0.0);
    vec4 diffuseColor = vec4(0.0, 0.0, 0.0, 0.0);
    vec4 specularColor = vec4(0.0, 0.0, 0.0, 0.0);
    if (diffuseFactor > 0.0) {
        diffuseColor =
                diffuseFactor
                    * gDirLight.diffuseIntensity
                    * vec4(gDirLight.color, 1.0)
                    * vec4(gMaterial.diffuse, 1.0);

        vec3 view = normalize(gCameraLocalPos - fs_in.localPos);
        vec3 halfway = normalize(gDirLight.direction + view);
        float specularFactor = pow(max(dot(halfway, view), 0.0), 16.0);
        specularColor =
                specularFactor
                    * vec4(gDirLight.color, 1.0)
                    * vec4(gMaterial.specular, 1.0);
    }
    return ambientColor + diffuseColor + specularColor;
}

void main() {
    if (gIsPBR) {
        color = calcPBRLighting();
    } else {
        color = calcBlinnPhongLighting();
    }
}