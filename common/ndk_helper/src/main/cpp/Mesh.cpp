#include "Mesh.hpp"

ndk_helper::mesh::Mesh::Mesh(
    std::vector<mesh::Vertex> vertices,
    std::vector<uint16_t> indices
) : vertices_{vertices}, indices_{indices} {
    prepare();
}

void ndk_helper::mesh::Mesh::prepare() {
    glGenVertexArrays(1, &vao_);
    glBindVertexArray(vao_);

    glGenBuffers(1, &vbo_);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_);
    glBufferData(
        GL_ARRAY_BUFFER,
        vertices_.size() * sizeof(Vertex),
        vertices_.data(),
        GL_STATIC_DRAW
    );

    glGenBuffers(1, &ebo_);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo_);
    glBufferData(
        GL_ELEMENT_ARRAY_BUFFER,
        indices_.size() * sizeof(uint16_t),
        indices_.data(),
        GL_STATIC_DRAW
    );

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(
        0,
        3,
        GL_FLOAT,
        GL_FALSE,
        sizeof(Vertex),
        nullptr
    );

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(
        1,
        3,
        GL_FLOAT,
        GL_FALSE,
        sizeof(Vertex),
        (void*)offsetof(Vertex, normal)
    );

    glEnableVertexAttribArray(2);
    glVertexAttribPointer(
        2,
        2,
        GL_FLOAT,
        GL_FALSE,
        sizeof(Vertex),
        (void*)offsetof(Vertex, texCoord)
    );
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(
        3,
        3,
        GL_FLOAT,
        GL_FALSE,
        sizeof(Vertex),
        (void*)offsetof(Vertex, tangent)
    );
    glEnableVertexAttribArray(4);
    glVertexAttribPointer(
        4,
        3,
        GL_FLOAT,
        GL_FALSE,
        sizeof(Vertex),
        (void*)offsetof(Vertex, bitangent)
    );

    glBindVertexArray(0);
}

bool ndk_helper::mesh::Mesh::operator==(const mesh::Mesh& other) const {
    return vertices_ == other.vertices_
           && indices_ == other.indices_;
}

void ndk_helper::mesh::Mesh::draw() const {
    glBindVertexArray(vao_);
    glDrawElements(GL_TRIANGLES, indices_.size(), GL_UNSIGNED_SHORT, 0);
    glBindVertexArray(0);
}
