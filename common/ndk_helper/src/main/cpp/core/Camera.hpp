#ifndef MESH_ROTATOR_CAMERA_HPP
#define MESH_ROTATOR_CAMERA_HPP

#include "../utils/GlmUtility.hpp"

namespace ndk_helper {
    namespace core {
        constexpr float DEFAULT_POS_X = 0.0f;
        constexpr float DEFAULT_POS_Y = 0.0f;
        constexpr float DEFAULT_POS_Z = 10.0f;
        constexpr float FOV_ANGLE = 45.0f;
        constexpr float Z_NEAR = 0.1f;
        constexpr float Z_FAR = 100.0f;

        struct Camera {
            Camera(glm::vec3 cameraPos, float screenWidth, float screenHeight);

            const glm::mat4 getViewMatrix() const;
            const glm::mat4 getProjectionMatrix() const;
            glm::vec3 getLocalPos(glm::mat4& modelMat) const;

            glm::vec3 position{DEFAULT_POS_X, DEFAULT_POS_Y, DEFAULT_POS_Z};
            glm::vec3 up{0.0f, 1.0f, 0.0f};
            glm::vec3 targetPos{0.0f, 0.0f, -1.0f};
            float width{0.0f};
            float height{0.0f};
        };

    }
}

#endif //MESH_ROTATOR_CAMERA_HPP
