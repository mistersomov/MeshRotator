#ifndef MESH_ROTATOR_ASSIMPUTILITY_HPP
#define MESH_ROTATOR_ASSIMPUTILITY_HPP

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <vector>
#include <string>
#include <assimp/scene.h>
#include <assimp/matrix4x4.h>

namespace ndk_helper {
    namespace utils {
        glm::vec3 toGlmVec3(const aiVector3D& v);
        glm::quat toGlmQuat(const aiQuaternion& q);
        glm::mat4 toGlmMat4(const aiMatrix4x4& m);
    }
}

#endif //MESH_ROTATOR_ASSIMPUTILITY_HPP
