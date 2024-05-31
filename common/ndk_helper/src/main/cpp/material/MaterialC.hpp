#ifndef MESH_ROTATOR_MATERIALC_HPP
#define MESH_ROTATOR_MATERIALC_HPP

#include "../core/Material.hpp"
#include "glm/glm.hpp"

namespace ndk_helper {
    namespace material {
        struct MaterialC : public core::Material {
            ~MaterialC() override = default;

            glm::vec3 ambient;
            glm::vec3 diffuse;
            glm::vec3 specular;
        };
    }
}

#endif //MESH_ROTATOR_MATERIALC_HPP
