#include "DirectionalLight.hpp"

ndk_helper::light::DirectionalLight::DirectionalLight(
    glm::vec3 direction,
    float diffuseIntensity,
    float ambientIntensity,
    glm::vec3 lightColor
) : CoreLight(ambientIntensity, lightColor),
    worldDirection{direction},
    diffuseIntensity{diffuseIntensity}
{

}

glm::vec3 ndk_helper::light::DirectionalLight::getLocalDirection(glm::mat4& modelMatrix) const {
    return glm::normalize(-worldDirection * ndk_helper::utils::mat4ToMat3Inversed(modelMatrix));
}
