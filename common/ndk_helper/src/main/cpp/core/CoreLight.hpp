#ifndef MESH_ROTATOR_CORELIGHT_HPP
#define MESH_ROTATOR_CORELIGHT_HPP

#include "../utils/GlmUtility.hpp"

namespace ndk_helper {
    namespace core {
        /**
         * A parent class of light for all light types.
         * This class contains only basic properties of light such as color and ambient intensity.
         * The derived classes will contain more properties of light such as position, direction and etc.
         */
        struct CoreLight {
            virtual ~CoreLight() = default;

            float ambientIntensity{0.1f};
            float diffuseIntensity{1.0f};
            glm::vec3 color = glm::vec3(1.0f);
        };
    }
}

#endif //MESH_ROTATOR_CORELIGHT_HPP
