#ifndef MESH_ROTATOR_MESHMOCK_HPP
#define MESH_ROTATOR_MESHMOCK_HPP

#include "../../Mesh.hpp"
#include <gmock/gmock.h>

namespace ndk_helper {
    namespace test {
        class MeshMock : public mesh::Mesh {
        public:
            MeshMock() : mesh::Mesh{{mesh::Vertex{{0.0f, 0.0f, 0.0f}}}, {}} {};

            MOCK_METHOD(void, draw, (shdr::Shader* shader), (const));
        };
    }
}

#endif //MESH_ROTATOR_MESHMOCK_HPP
