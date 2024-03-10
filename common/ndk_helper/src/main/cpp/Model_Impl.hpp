#ifndef MESH_ROTATOR_MODEL_IMPL_HPP
#define MESH_ROTATOR_MODEL_IMPL_HPP

#include "Model.hpp"

namespace ndk_helper {
    namespace mdl {
        class Model_Impl : public Model {
        public:
            Model_Impl(
                const mesh::Mesh& mesh,
                const glm::vec3& position = {0.0f, 0.0f, 0.0f},
                const glm::vec3& scale = {1.0f, 1.0f, 1.0f}
            );
            ~Model_Impl() override = default;

            void addMesh(mesh::Mesh& mesh);
            std::vector<mesh::Mesh> getMeshes() const override;
            void draw(shdr::Shader& shader) const override;
            void setPosition(const glm::vec3& position) override;
            glm::vec3 getPosition() const override;
            void setScale(const glm::vec3& scale) override;
            glm::vec3 getScale() const override;

        private:
            std::vector<mesh::Mesh> meshes_;
            glm::vec3 position_;
            glm::vec3 scale_;
        };
    }
}

#endif //MESH_ROTATOR_MODEL_IMPL_HPP
