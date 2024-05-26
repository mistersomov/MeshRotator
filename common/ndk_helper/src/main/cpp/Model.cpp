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

void ndk_helper::mdl::Model::addTexture(ndk_helper::mesh::Texture texture) {
    textures_.emplace_back(texture);
}

void ndk_helper::mdl::Model::draw(ndk_helper::shdr::Shader& shader) const {
    for (auto i = 0; i != textures_.size(); ++i) {
        glActiveTexture(GL_TEXTURE0 + i);
        std::string name;
        if (textures_[i].type == mesh::TextureType::DIFFUSE) {
            name = "albedoT";
        } else if (textures_[i].type == mesh::TextureType::NORMAL) {
            name = "normalT";
        } else if (textures_[i].type == mesh::TextureType::METALLIC) {
            name = "metallicT";
        } else if (textures_[i].type == mesh::TextureType::ROUGHNESS) {
            name = "roughnessT";
        } else if (textures_[i].type == mesh::TextureType::AO) {
            name = "aoT";
        }
        //shader.set_int("gMaterial." + name, i);
        shader.set_int("gPBRMaterial." + name, i);
        glBindTexture(GL_TEXTURE_2D, textures_[i].id);
    }
    std::for_each(meshes_.begin(), meshes_.end(), [&](const mesh::Mesh &mesh) {
        mesh.draw();
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
