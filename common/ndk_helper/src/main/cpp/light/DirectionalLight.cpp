#include "DirectionalLight.hpp"

ndk_helper::light::DirectionalLight::DirectionalLight(glm::vec3 direction) : Light(), worldDirection{direction}
{

}

glm::vec3 ndk_helper::light::DirectionalLight::getLocalDirection(glm::mat4& modelMatrix) const {
    return glm::normalize(-worldDirection * ndk_helper::utils::mat4ToMat3Inversed(modelMatrix));
}
