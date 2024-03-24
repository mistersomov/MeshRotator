#ifndef MESHROTATOR_MODEL_HPP
#define MESHROTATOR_MODEL_HPP

#include "Mesh.hpp"
#include "Shader.hpp"

namespace ndk_helper {
    namespace mdl {
        class Model {
        public:
            Model() = default;
            Model(
                const mesh::Mesh& mesh,
                const glm::vec3& position = {0.0f, 0.0f, 0.0f},
                const glm::vec3& scale = {1.0f, 1.0f, 1.0f}
            );

            void addMesh(mesh::Mesh& mesh);
            std::vector<mesh::Mesh> getMeshes() const;
            void addTexture(mesh::Texture texture);
            void draw(shdr::Shader& shader) const;
            void setPosition(const glm::vec3& position);
            glm::vec3 getPosition() const;
            void setScale(const glm::vec3& scale);
            glm::vec3 getScale() const;

        private:
            std::vector<mesh::Mesh> meshes_;
            std::vector<mesh::Texture> textures_;
            glm::vec3 position_;
            glm::vec3 scale_;
        };
    }
}

#endif //MESHROTATOR_MODEL_HPP