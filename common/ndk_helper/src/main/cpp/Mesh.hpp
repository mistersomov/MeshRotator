#ifndef MESHROTATOR_MESH_HPP
#define MESHROTATOR_MESH_HPP

#include "Shader.hpp"

#include <vector>
#include <string>

namespace ndk_helper {
    namespace mesh {
        struct Vertex {
            glm::vec3 position;
            glm::vec3 normal;
            glm::vec2 texCoord;
            glm::vec3 tangent;
            glm::vec3 bitangent;

            bool operator==(const mesh::Vertex& other) const {
                return position == other.position
                    && normal == other.normal
                    && texCoord == other.texCoord
                    && tangent == other.tangent
                    && bitangent == other.bitangent;
            };
        };

        struct Texture {
            uint32_t id;
            std::string type;

            bool operator==(const mesh::Texture& other) const {
                return id == other.id
                    && type == other.type;
            };
        };

        class Mesh {
            GLuint vao_{0};
            GLuint vbo_{0};
            GLuint ebo_{0};

            std::vector<Vertex> vertices_;
            std::vector<uint16_t> indices_;
            std::vector<Texture> textures_;

            void prepare();

        public:
            Mesh(std::vector<mesh::Vertex>, std::vector<uint16_t>);
            Mesh(std::vector<mesh::Vertex>, std::vector<uint16_t>, std::vector<Texture>);

            bool operator==(const mesh::Mesh& other) const;

            void draw(ndk_helper::shdr::Shader*) const;
        };
    }
}

#endif //MESHROTATOR_MESH_HPP
