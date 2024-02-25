#ifndef MESHROTATOR_MODEL_HPP
#define MESHROTATOR_MODEL_HPP

#include "Mesh.hpp"

#include <assimp/scene.h>
#include <vector>
#include <string>

namespace ndk_helper {
    namespace mdl {
        class Model {
        public:
            Model(ndk_helper::mesh::Mesh);
            Model(std::vector<ndk_helper::mesh::Mesh>);
            Model(const std::vector<GLfloat>&, const std::vector<uint16_t>&);
            Model(
                const std::vector<GLfloat>&,
                const std::vector<uint16_t>&,
                const std::vector<mesh::Texture>&
            );

            void draw(shdr::Shader&) const;

        private:
            std::vector<mesh::Mesh> meshes_;
            std::vector<mesh::Texture> textures_;

            void process_mesh(aiMesh*);
        };
    }
}

#endif //MESHROTATOR_MODEL_HPP