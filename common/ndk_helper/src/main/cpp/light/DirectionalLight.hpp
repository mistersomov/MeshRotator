#ifndef MESH_ROTATOR_DIRECTIONALLIGHT_HPP
#define MESH_ROTATOR_DIRECTIONALLIGHT_HPP

#include "../core/CoreLight.hpp"

namespace ndk_helper {
    namespace light {
        struct DirectionalLight : public core::CoreLight {
            DirectionalLight(
                glm::vec3 direction,
                float diffuseIntensity,
                float ambientIntensity,
                glm::vec3 lightColor = glm::vec3{1.0f}
            );
            ~DirectionalLight() = default;

            glm::vec3 getLocalDirection(glm::mat4& modelMatrix) const;

            glm::vec3 worldDirection;
            float diffuseIntensity{0.0f};
        };
    }
}

#endif //MESH_ROTATOR_DIRECTIONALLIGHT_HPP
