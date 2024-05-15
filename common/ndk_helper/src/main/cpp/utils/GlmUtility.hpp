#ifndef MESH_ROTATOR_GLMUTILITY_HPP
#define MESH_ROTATOR_GLMUTILITY_HPP

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace ndk_helper {
    namespace utils {
        glm::mat3 mat4ToMat3Inversed(const glm::mat4& m);
    }
}

#endif //MESH_ROTATOR_GLMUTILITY_HPP
