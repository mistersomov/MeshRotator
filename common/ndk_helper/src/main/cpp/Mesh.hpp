#ifndef MESHROTATOR_MESH_HPP
#define MESHROTATOR_MESH_HPP

#include <glm/glm.hpp>
#include <GLES3/gl32.h>
#include <vector>

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

        enum class TextureType {
            NONE,
            DIFFUSE,
            NORMAL
        };

        struct Texture {
            TextureType type;
            uint32_t id;

            bool operator==(const Texture& other) const {
                return static_cast<uint32_t>(type) == static_cast<uint32_t>(other.type)
                    && id == other.id;
            };
        };

        class Mesh {
        public:
            Mesh(std::vector<Vertex>, std::vector<uint16_t>);

            bool operator==(const Mesh& other) const;

            void draw() const;

        private:
            GLuint vao_{0};
            GLuint vbo_{0};
            GLuint ebo_{0};

            std::vector<Vertex> vertices_;
            std::vector<uint16_t> indices_;

            void prepare();
        };
    }
}

#endif //MESHROTATOR_MESH_HPP
