#ifndef MESHROTATOR_MODEL_HPP
#define MESHROTATOR_MODEL_HPP

#include "Mesh.hpp"

namespace ndk_helper {
    namespace mdl {
        struct Model {
            virtual ~Model() {};

            virtual std::vector<mesh::Mesh> getMeshes() const = 0;
            virtual void draw(shdr::Shader& shader) const = 0;
            virtual void setPosition(const glm::vec3& position) = 0;
            virtual glm::vec3 getPosition() const = 0;
            virtual void setScale(const glm::vec3& scale) = 0;
            virtual glm::vec3 getScale() const = 0;
        };
    }
}

#endif //MESHROTATOR_MODEL_HPP