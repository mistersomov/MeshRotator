#ifndef MESH_ROTATOR_TRANSFORMATION_HPP
#define MESH_ROTATOR_TRANSFORMATION_HPP

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace ndk_helper {
    namespace core {
        struct Transformation {
            glm::vec3 position{0.0f, 0.0f, -1.0f};
            glm::vec3 rotation{1.0f};
            glm::vec3 scale{1.0f};
        };
    }
}

#endif //MESH_ROTATOR_TRANSFORMATION_HPP
