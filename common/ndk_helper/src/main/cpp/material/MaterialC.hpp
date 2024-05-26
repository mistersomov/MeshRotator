#ifndef MESH_ROTATOR_MATERIALC_HPP
#define MESH_ROTATOR_MATERIALC_HPP

#include "glm/glm.hpp"

namespace ndk_helper {
    namespace core {
        struct MaterialC {
            glm::vec3 ambient;
            glm::vec3 diffuse;
            glm::vec3 specular;
        };
    }
}

#endif //MESH_ROTATOR_MATERIALC_HPP
