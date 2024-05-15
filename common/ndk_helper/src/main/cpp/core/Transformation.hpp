#ifndef MESH_ROTATOR_TRANSFORMATION_HPP
#define MESH_ROTATOR_TRANSFORMATION_HPP

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>

namespace ndk_helper {
    namespace core {
        struct Transformation {
            glm::vec3 position{0.0f, 0.0f, -0.1f};
            glm::quat rotation{1.0f, 0.0f, 0.0f, 0.0f};
            glm::vec3 scale{1.0f};

            glm::mat4 getModelMatrix() const {
                glm::mat4 scaleMat = glm::scale(glm::mat4(1.0f), scale);
                glm::mat4 translateMat = glm::translate(glm::mat4(1.0f), position);

                return translateMat * glm::mat4_cast(rotation) * scaleMat;
            }
        };
    }
}

#endif //MESH_ROTATOR_TRANSFORMATION_HPP
