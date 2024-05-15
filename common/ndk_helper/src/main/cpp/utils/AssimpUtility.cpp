#include "AssimpUtility.hpp"

glm::vec3 ndk_helper::utils::toGlmVec3(const aiVector3D& v) {
    return glm::vec3(v.x, v.y, v.z);
}

glm::quat ndk_helper::utils::toGlmQuat(const aiQuaternion& q) {
    return glm::quat(q.w, q.x, q.y, q.z);
}

glm::mat4 ndk_helper::utils::toGlmMat4(const aiMatrix4x4& m) {
    return glm::mat4(
        m.a1, m.b1, m.c1, m.d1,
        m.a2, m.b2, m.c2, m.d2,
        m.a3, m.b3, m.c3, m.d3,
        m.a4, m.b4, m.c4, m.d4
    );
}