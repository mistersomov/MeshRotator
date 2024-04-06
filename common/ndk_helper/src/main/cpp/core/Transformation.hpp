#ifndef MESH_ROTATOR_TRANSFORMATION_HPP
#define MESH_ROTATOR_TRANSFORMATION_HPP

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace ndk_helper {
    namespace core {
        struct Transformation {
            //TODO think about position values
            glm::vec3 position_{0.0f, 0.0f, -12.0f};
            glm::vec3 rotation_{0.0f};
            glm::vec3 scale_{1.0f};
        };
    }
}

#endif //MESH_ROTATOR_TRANSFORMATION_HPP
