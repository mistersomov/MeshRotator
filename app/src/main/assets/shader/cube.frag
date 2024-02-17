#version 300 es

precision mediump float;

struct Material {
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
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

//in VS_OUT {
//    vec3 normal;
//    vec3 fragPos;
//    vec2 texCoords;
//} fs_in;

in vec3 normal;
in vec3 fragPos;
in vec2 texCoords;

out vec4 color;

uniform vec3 viewPos;

uniform Material material;
uniform PointLight pointLight;
uniform DirLight dirLight;

float get_attenuation(PointLight light, float distance);
float get_point_light_distance(PointLight light, vec3 pos);
vec3 calc_point_light(PointLight light, vec3 normal, vec3 viewDir, vec3 fragPos);
vec3 calc_dir_light(DirLight light, vec3 normal, vec3 viewDir);

void main() {
    vec3 norm = normalize(normal);
    vec3 viewDir = normalize(viewPos - fragPos);

    vec3 result = calc_point_light(pointLight, norm, viewDir, fragPos);
    result += calc_dir_light(dirLight, norm, viewDir);

    color = vec4(result, 1.0);
}

float get_attenuation(PointLight light, float distance) {
    return 1.0 / (light.constant + light.linear * distance + light.quadratic * distance * distance);
}

float get_point_light_distance(PointLight light, vec3 pos) {
    return length(light.position - pos);
}

vec3 calc_point_light(PointLight light, vec3 normal, vec3 viewDir, vec3 fragPos) {
    vec3 lightDir = normalize(light.position - fragPos);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    // attenuation
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
    // combine results
    vec3 ambient = light.ambient * vec3(texture(material.diffuse, texCoords));
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, texCoords));
    vec3 specular = light.specular * spec * vec3(texture(material.specular, texCoords));
    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;
    return (ambient + diffuse + specular);
}

// calculates the color when using a directional light.
vec3 calc_dir_light(DirLight light, vec3 normal, vec3 viewDir) {
    vec3 lightDir = normalize(-light.direction);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    // combine results
    vec3 ambient = light.ambient * vec3(texture(material.diffuse, texCoords));
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, texCoords));
    vec3 specular = light.specular * spec * vec3(texture(material.specular, texCoords));
    return (ambient + diffuse + specular);
}