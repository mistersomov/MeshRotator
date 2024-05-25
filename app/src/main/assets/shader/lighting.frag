#version 320 es

precision mediump float;
precision mediump int;

#define PI 3.1415926535897932384626433832795
#define GAMMA 2.2

struct Material {
    vec3 ambient;
    sampler2D diffuseT;
    vec3 specular;
    sampler2D normalT;
};

struct PBRMaterial {
    sampler2D albedoT;
    sampler2D normalT;
    sampler2D metallicT;
    sampler2D roughnessT;
    sampler2D aoT;
    bool isMetal;
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
    vec3 tangent;
} fs_in;

// Materials
uniform bool gIsPBR;
uniform PBRMaterial gPBRMaterial;
uniform Material gMaterial;

// Lights
const int MAX_POINT_LIGHTS = 4;
const float kShininess = 16.0;
const float kEnergyConservation = (8.0 + kShininess) / (8.0 * PI);
uniform DirLight gDirLight;
uniform int gNumPointLights;
uniform PointLight gPointLights[MAX_POINT_LIGHTS];

uniform vec3 gCameraLocalPos;

out vec4 color;

vec3 calcNormal(bool isPBR) {
    vec3 nNomalized = normalize(fs_in.normal);
    vec3 tangent = normalize(fs_in.tangent);
    tangent = normalize(tangent - dot(tangent, nNomalized) * nNomalized);
    vec3 bitangent = cross(tangent, nNomalized);
    vec3 normalBumpMap = vec3(0.0);

    if (isPBR) {
        normalBumpMap = texture(gPBRMaterial.normalT, fs_in.texCoords).rgb;
    } else {
        normalBumpMap = texture(gMaterial.normalT, fs_in.texCoords).rgb;
    }
    normalBumpMap = 2.0 * normalBumpMap - vec3(1.0);
    mat3 TBN = mat3(tangent, bitangent, nNomalized);

    return normalize(TBN * normalBumpMap);
}

// sRGB to linear approximation.
vec3 sRgb2Lin(vec3 sRGB) {
    return pow(sRGB, vec3(GAMMA));
}

// linear to sRGB approximation.
vec3 lin2sRgb(vec3 linear) {
    return pow(linear, vec3(1.0 / GAMMA));
}

// This is D(h) component from Cook-Torrance equation,
// where h is halfway vector between light position vector and view position vector.
// D(h) = alpha^2 / (pi * (n * h)^2 * (alpha^2 - 1) + 1)^2,
// where alpha^2 is PBR material's squared roughness.
float ggxDistribution(float nDotH, float roughness) {
    float alpha2 = pow(roughness, 2.0);
    float denominator = max(pow(nDotH, 2.0) * (alpha2 - 1.0) + 1.0, 0.000001);
    return alpha2 / (PI * pow(denominator, 2.0));
}

// This is G(dp) component from Cook-Torrance equation,
// where dp is dot poduct between normal vector and light vector
// also between normal vector and view position vector.
// G(dp) = dp / (dp * (1 - k) + 1),
// where k is PBR material's squared (roughness + 1) divided by 8.
float geomSmith(float dotProduct, float roughness) {
    float k = pow(roughness + 1.0, 2.0) / 8.0;
    float denominator = max(dotProduct * (1.0 - k) + k, 0.000001);
    return dotProduct / denominator;
}

// This is F(v, h) component from Cook-Torrance equation, which named Fresnel.
// F(v, h) = F0 + (1.0 - F0) * pow(clamp(1.0 - vDotH, 0.0, 1.0), 5.0),
// where F0 is dielectric/metal roughness of PBR material. See a table of roughness from internet.
vec3 shlickFresnel(float vDotH, vec3 albedo, float metallic) {
    vec3 F0 = mix(vec3(0.04), albedo, metallic);
    return F0 + (1.0 - F0) * pow(clamp(1.0 - vDotH, 0.0, 1.0), 5.0);
}

// Cook-Torrance microfacet specular shading model is
// f(l, v) = D(h) * G(l, v, h) * F(v, h) / 4 (n * l) (n * v)
vec3 calcPBRLighting(CoreLight core, vec3 position, bool isDirLight, vec3 normal) {
    vec3 albedo = sRgb2Lin(texture(gPBRMaterial.albedoT, fs_in.texCoords).rgb);
    float roughness = texture(gPBRMaterial.roughnessT, fs_in.texCoords).r;
    float metallic = texture(gPBRMaterial.metallicT, fs_in.texCoords).r;
    float ao = texture(gPBRMaterial.aoT, fs_in.texCoords).r;

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

    vec3 kS = shlickFresnel(vDotH, albedo, metallic);
    vec3 kD = 1.0 - kS;
    kD *= 1.0 - metallic;

    // ambient
    vec3 ambient = vec3(0.03) * albedo * ao;
    // diffuse
    vec3 fLambert = vec3(0.0);
    if (!gPBRMaterial.isMetal) {
        fLambert = albedo;
    }
    vec3 diffuseBRDF = kD * fLambert / PI;
    // specular
    vec3 specBRDFNom =
            ggxDistribution(nDotH, roughness)
                * geomSmith(nDotL, roughness)
                * geomSmith(nDotV, roughness)
                * kS;
    float specBRDFDenom = max(4.0 * nDotL * nDotV, 0.0001);
    vec3 specularBRDF = specBRDFNom / specBRDFDenom;

    return ambient + (diffuseBRDF + specularBRDF) * lightIntensity * nDotL;
}

vec3 calcBlinnPhongLighting(CoreLight core, vec3 lightPos, vec3 normal) {
    vec3 ambientColor =
            core.ambientIntensity
                * core.color
                * gMaterial.ambient;

    vec3 diffuse = texture(gMaterial.diffuseT, fs_in.texCoords).rgb;
    float diffuseFactor = max(dot(lightPos, normal), 0.0);
    vec3 diffuseColor = vec3(0.0, 0.0, 0.0);
    vec3 specularColor = vec3(0.0, 0.0, 0.0);
    if (diffuseFactor > 0.0) {
        diffuseColor =
                diffuseFactor
                    * diffuse
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
    vec3 normal = calcNormal(isPBR);
    vec3 total = vec3(0.0, 0.0, 0.0);

    if (isPBR) {
        total = calcPBRLighting(gDirLight.core, gDirLight.direction, true, normal);
        for (int i = 0; i != gNumPointLights; ++i) {
            total += calcPBRLighting(gPointLights[i].core, gPointLights[i].position, false, normal);
        }
    } else {
        total = calcBlinnPhongLighting(gDirLight.core, gDirLight.direction, normal);
        for (int i = 0; i != gNumPointLights; ++i) {
            total += calcBlinnPhongPointLighting(gPointLights[i], normal);
        }
    }

    return total;
}

void main() {
    vec3 result = calcLightingInternal(gIsPBR);
    result = result / (result + vec3(1.0));
    result = lin2sRgb(result);
    color = vec4(result, 1.0);
}