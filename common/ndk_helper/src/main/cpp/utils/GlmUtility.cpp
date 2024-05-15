#include "GlmUtility.hpp"

glm::mat3 ndk_helper::utils::mat4ToMat3Inversed(const glm::mat4& m) {
    glm::mat3 topLeftMatrix = glm::mat3(
        m[0][0], m[0][1], m[0][2],
        m[1][0], m[1][1], m[1][2],
        m[2][0], m[2][1], m[2][2]
    );
    return glm::transpose(glm::inverse(topLeftMatrix));
}
