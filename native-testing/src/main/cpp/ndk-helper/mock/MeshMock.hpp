#ifndef MESH_ROTATOR_MESHMOCK_HPP
#define MESH_ROTATOR_MESHMOCK_HPP

#include "Mesh.hpp"
#include <gmock/gmock.h>

namespace tests {
    using Vertex = ndk_helper::mesh::Vertex;
    using Mesh = ndk_helper::mesh::Mesh;
    using Shader = ndk_helper::shdr::Shader;

    class MeshMock : public Mesh {
    public:
        MeshMock() : Mesh{{Vertex{
            {0.0f, 0.0f, 0.0f},
            {0.0f, 0.0f, 0.0f},
            {0.0f, 0.0f},
            {0.0f, 0.0f, 0.0f},
            {0.0f, 0.0f, 0.0f}
        }}, {}} {}

        MOCK_METHOD(void, draw, (Shader * shader), (const));
    };
}

#endif //MESH_ROTATOR_MESHMOCK_HPP
