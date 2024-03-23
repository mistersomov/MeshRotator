#include "Model.hpp"
#include "NDKHelper.hpp"

ndk_helper::mdl::Model::Model(
        const ndk_helper::mesh::Mesh& mesh,
        const glm::vec3& position,
        const glm::vec3& scale
) : position_{position}, scale_{scale} {
    meshes_.push_back(mesh);
}

void ndk_helper::mdl::Model::addMesh(mesh::Mesh& mesh) {
    meshes_.emplace_back(mesh);
}

std::vector<ndk_helper::mesh::Mesh> ndk_helper::mdl::Model::getMeshes() const {
    return meshes_;
}

void ndk_helper::mdl::Model::draw(ndk_helper::shdr::Shader& shader) const {
    std::for_each(meshes_.begin(), meshes_.end(), [&](const mesh::Mesh &mesh) {
        mesh.draw(&shader);
    });
}

void ndk_helper::mdl::Model::setPosition(const glm::vec3& position) {
    position_ = position;
}

glm::vec3 ndk_helper::mdl::Model::getPosition() const {
    return position_;
}

void ndk_helper::mdl::Model::setScale(const glm::vec3& scale) {
    scale_ = scale;
}

glm::vec3 ndk_helper::mdl::Model::getScale() const {
    return scale_;
}
