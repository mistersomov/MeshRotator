#ifndef MESH_ROTATOR_DIRECTIONALLIGHT_HPP
#define MESH_ROTATOR_DIRECTIONALLIGHT_HPP

#include "../core/Light.hpp"

namespace ndk_helper {
    namespace light {
        struct DirectionalLight : public core::Light {
            DirectionalLight(glm::vec3 direction);
            ~DirectionalLight() override = default;

            glm::vec3 getLocalDirection(glm::mat4& modelMatrix) const;

            glm::vec3 worldDirection;
        };
    }
}

#endif //MESH_ROTATOR_DIRECTIONALLIGHT_HPP
