#include "Model_Impl.hpp"
#include "../../../../../app/src/main/cpp/utils/AndroidOut.h"

ndk_helper::mdl::Model_Impl::Model_Impl(
    const ndk_helper::mesh::Mesh& mesh,
    const glm::vec3& position,
    const glm::vec3& scale
) : position_{position}, scale_{scale} {
    meshes_.push_back(mesh);
}

void ndk_helper::mdl::Model_Impl::addMesh(mesh::Mesh& mesh) {
    meshes_.emplace_back(mesh);
}

std::vector<ndk_helper::mesh::Mesh> ndk_helper::mdl::Model_Impl::getMeshes() const {
    return meshes_;
}

void ndk_helper::mdl::Model_Impl::draw(ndk_helper::shdr::Shader& shader) const {
    std::for_each(meshes_.begin(), meshes_.end(), [&](const mesh::Mesh &mesh) {
        mesh.draw(&shader);
    });
}

void ndk_helper::mdl::Model_Impl::setPosition(const glm::vec3& position) {
    position_ = position;
}

glm::vec3 ndk_helper::mdl::Model_Impl::getPosition() const {
    return position_;
}

void ndk_helper::mdl::Model_Impl::setScale(const glm::vec3& scale) {
    scale_ = scale;
}

glm::vec3 ndk_helper::mdl::Model_Impl::getScale() const {
    return scale_;
}
