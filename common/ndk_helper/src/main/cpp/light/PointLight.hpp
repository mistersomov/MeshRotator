#ifndef MESH_ROTATOR_POINTLIGHT_HPP
#define MESH_ROTATOR_POINTLIGHT_HPP

#include "../core/CoreLight.hpp"
#include "../utils/GlmUtility.hpp"

namespace ndk_helper {
    namespace light {
        struct PointLight : public core::CoreLight {
            PointLight(glm::vec3 lightPos);
            ~PointLight() override = default;

            glm::vec3 getLocalPos(glm::mat4& modelMatrix) const;

            float aConstant{1.0f};
            float aLinear{0.0f};
            float aQuadratic{0.0f};
            glm::vec3 position;
        };
    }
}

#endif //MESH_ROTATOR_POINTLIGHT_HPP
