#include "PointLight.hpp"

ndk_helper::light::PointLight::PointLight(glm::vec3 lightPos) : Light(), position{lightPos} {

}

glm::vec3 ndk_helper::light::PointLight::getLocalPos(glm::mat4& modelMatrix) const {
    return position * ndk_helper::utils::mat4ToMat3Inversed(modelMatrix);
}
