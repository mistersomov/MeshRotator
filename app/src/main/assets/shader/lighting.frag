#version 320 es

precision mediump float;

#define PI 3.1415926535897932384626433832795
#define GAMMA 2.2

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

struct CoreLight {
    vec3 color;
    float ambientIntensity;
    float diffuseIntensity;
};

struct DirLight {
    CoreLight core;
    vec3 direction;
};

struct Attenuation {
    float constant;
    float linear;
    float quadratic;
};

struct PointLight {
    CoreLight core;
    Attenuation attenuation;
    vec3 position; // must be in local space
};

in VS_OUT {
    vec3 localPos;
    vec2 texCoords;
    vec3 normal;
} fs_in;

// Materials
uniform bool gIsPBR;
uniform PBRMaterial gPBRMaterial;
uniform Material gMaterial;

// Lights
const int MAX_POINT_LIGHTS = 4;
uniform DirLight gDirLight;
uniform int gNumPointLights;
uniform PointLight gPointLights[MAX_POINT_LIGHTS];
const float kShininess = 16.0;
const float kEnergyConservation = (8.0 + kShininess) / (8.0 * PI);

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
vec3 calcPBRLighting(CoreLight core, vec3 position, bool isDirLight, vec3 normal) {
    vec3 lightIntensity = core.color * core.diffuseIntensity;
    vec3 lightDir = vec3(0.0);

    if (isDirLight) {
        lightDir = position;
    } else {
        lightDir = position - fs_in.localPos;
        float lightToPixelDist = length(lightDir);
        lightDir = normalize(lightDir);
        lightIntensity /= pow(lightToPixelDist, 2.0);
    }

    vec3 view = normalize(gCameraLocalPos - fs_in.localPos);
    vec3 halfway = normalize(view + lightDir);

    float nDotH = max(dot(normal, halfway), 0.0);
    float vDotH = max(dot(view, halfway), 0.0);
    float nDotL = max(dot(normal, lightDir), 0.0);
    float nDotV = max(dot(normal, view), 0.0);

    vec3 kS = shlickFresnel(vDotH);
    vec3 kD = 1.0 - kS;

    vec3 specBRDFNom =
            ggxDistribution(nDotH)
                * geomSmith(nDotL)
                * geomSmith(nDotV)
                * kS;
    float specBRDFDenom = max(4.0 * nDotL * nDotV, 0.0001);
    vec3 specularBRDF = specBRDFNom / specBRDFDenom;

    vec3 fLambert = vec3(0.0);
    if (!gPBRMaterial.isMetal) {
        fLambert = gPBRMaterial.color;
    }
    vec3 diffuseBRDF = kD * fLambert / PI;

    return (diffuseBRDF + specularBRDF) * lightIntensity * nDotL;
}

vec3 calcBlinnPhongLighting(CoreLight core, vec3 lightPos, vec3 normal) {
    vec3 ambientColor =
            core.ambientIntensity
                * core.color
                * gMaterial.ambient;

    float diffuseFactor = max(dot(lightPos, normal), 0.0);
    vec3 diffuseColor = vec3(0.0, 0.0, 0.0);
    vec3 specularColor = vec3(0.0, 0.0, 0.0);
    if (diffuseFactor > 0.0) {
        diffuseColor =
                diffuseFactor
                    * gMaterial.diffuse
                    * core.diffuseIntensity
                    * core.color;

        vec3 view = normalize(gCameraLocalPos - fs_in.localPos);
        vec3 halfway = normalize(lightPos + view);
        float specularFactor = kEnergyConservation * pow(max(dot(halfway, normal), 0.0), kShininess);
        specularColor =
                specularFactor
                    * gMaterial.specular
                    * core.diffuseIntensity
                    * core.color;
    }

    return ambientColor + diffuseColor + specularColor;
}

vec3 calcBlinnPhongPointLighting(PointLight light, vec3 normal) {
    vec3 lightDir = light.position - fs_in.localPos;
    float d = length(lightDir);
    lightDir = normalize(lightDir);
    vec3 color = calcBlinnPhongLighting(light.core, lightDir, normal);
    float attentuation =
            light.attenuation.constant
                + light.attenuation.linear * d
                + light.attenuation.quadratic * pow(d, 2.0);

    return color / attentuation;
}

vec3 calcLightingInternal(bool isPBR) {
    vec3 nNormalized = normalize(fs_in.normal);
    vec3 total = vec3(0.0, 0.0, 0.0);

    if (isPBR) {
        total = calcPBRLighting(gDirLight.core, gDirLight.direction, true, nNormalized);
        for (int i = 0; i != gNumPointLights; ++i) {
            total += calcPBRLighting(gPointLights[i].core, gPointLights[i].position, false, nNormalized);
        }
    } else {
        total = calcBlinnPhongLighting(gDirLight.core, gDirLight.direction, nNormalized);
        for (int i = 0; i != gNumPointLights; ++i) {
            total += calcBlinnPhongPointLighting(gPointLights[i], nNormalized);
        }
    }

    return total;
}

void main() {
    color = vec4(pow(calcLightingInternal(gIsPBR), vec3(1.0 / GAMMA)), 1.0);
}