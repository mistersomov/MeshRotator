#ifndef MESHROTATOR_MODEL_HPP
#define MESHROTATOR_MODEL_HPP

#include "Mesh.hpp"

#include <assimp/scene.h>
#include <vector>
#include <string>

namespace ndk_helper {
    namespace mdl {
        class Model {
            std::vector<ndk_helper::mesh::Mesh> meshes_;

            void process_mesh(aiMesh*);
        public:
            Model(const std::vector<aiMesh*>&);
            Model(const std::vector<GLfloat>&, const std::vector<uint16_t>&);
            Model(
                const std::vector<GLfloat>&,
                const std::vector<uint16_t>&,
                const std::vector<mesh::Texture>&
            );

            void draw(shdr::Shader&) const;

        };
    }
}

#endif //MESHROTATOR_MODEL_HPP
