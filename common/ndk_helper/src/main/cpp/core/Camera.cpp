#include "Camera.hpp"
#include "../utils/AndroidOut.hpp"

ndk_helper::core::Camera::Camera(
    glm::vec3 cameraPos,
    float screenWidth,
    float screenHeight
) : position{cameraPos},
    width{screenWidth},
    height{screenHeight}
{

}

const glm::mat4 ndk_helper::core::Camera::getViewMatrix() const {
    return glm::lookAt(
        position,
        targetPos,
        up
    );
}

const glm::mat4 ndk_helper::core::Camera::getProjectionMatrix() const {
    if (width == 0.0f || height == 0.0f) {
        aout << "Camera error: width or height is 0.0f. Try to set correct width and height" << std::endl;
        return glm::mat4(1.0f);
    }
    return glm::perspective(
        FOV_ANGLE,
        width / height,
        Z_NEAR,
        Z_FAR
    );
}

glm::vec3 ndk_helper::core::Camera::getLocalPos(glm::mat4& modelMat) const {
    return position * ndk_helper::utils::mat4ToMat3Inversed(modelMat);
}
