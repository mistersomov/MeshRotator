#version 320 es

precision mediump float;

struct Material {
    sampler2D diffuse;
    sampler2D specular;
    sampler2D normal;
};

struct PointLight {
    vec3 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;
};

struct DirLight {
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

in VS_OUT {
    vec3 fragPos;
    vec2 texCoords;
    mat3 TBN;
} fs_in;

out vec4 color;

uniform vec3 viewPos;

uniform Material material;
uniform PointLight pointLight;
uniform DirLight dirLight;

const float pi = 3.14159265;
const float shininess = 16.0;
const float energyConservation = (8.0 + shininess) / (8.0 * pi);

float get_attenuation(PointLight light, float distance);
float get_point_light_distance(PointLight light, vec3 pos);
vec3 calc_point_light(PointLight light, vec3 normal, vec3 viewDir, vec3 fragPos);
vec3 calc_dir_light(DirLight light, vec3 normal, vec3 viewDir);

void main() {
    vec3 normal = texture(material.normal, fs_in.texCoords).rgb;
    normal = normalize(normal * 2.0 - 1.0);
    vec3 viewDir = fs_in.TBN * normalize(viewPos - fs_in.fragPos);
    vec3 result = calc_point_light(pointLight, normal, viewDir, fs_in.fragPos);

    result += calc_dir_light(dirLight, normal, viewDir);
    color = vec4(result, 1.0);
}

float get_attenuation(PointLight light, float distance) {
    return 1.0 / (light.constant + light.linear * distance + light.quadratic * distance * distance);
}

float get_point_light_distance(PointLight light, vec3 pos) {
    return length(light.position - pos);
}

vec3 calc_point_light(PointLight light, vec3 normal, vec3 viewDir, vec3 fragPos) {
    vec3 lightDir = fs_in.TBN * normalize(light.position - fragPos);
    // unit vector exactly halfway between the light direction and the view direction
    vec3 halfway = normalize(lightDir + viewDir);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    float spec = energyConservation * pow(max(dot(normal, halfway), 0.0), shininess);
    // attenuation
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
    // combine results
    vec3 ambient = light.ambient * vec3(texture(material.diffuse, fs_in.texCoords));
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, fs_in.texCoords));
    vec3 specular = light.specular * spec * vec3(texture(material.specular, fs_in.texCoords));

    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;

    return ambient + diffuse + specular;
}

// calculates the color when using a directional light.
vec3 calc_dir_light(DirLight light, vec3 normal, vec3 viewDir) {
    vec3 lightDir = fs_in.TBN * normalize(-light.direction);
    // unit vector exactly halfway between the light direction and the view direction
    vec3 halfway = normalize(lightDir + viewDir);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    float spec = energyConservation * pow(max(dot(normal, halfway), 0.0), shininess);
    // combine results
    vec3 ambient = light.ambient * vec3(texture(material.diffuse, fs_in.texCoords));
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, fs_in.texCoords));
    vec3 specular = light.specular * spec * vec3(texture(material.specular, fs_in.texCoords));

    return ambient + diffuse + specular;
}