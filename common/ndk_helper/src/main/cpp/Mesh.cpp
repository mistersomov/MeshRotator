#include "Mesh.hpp"

ndk_helper::mesh::Mesh::Mesh(
    std::vector<mesh::Vertex> vertices,
    std::vector<uint16_t> indices
) : vertices_{vertices}, indices_{indices} {
    prepare();
}

ndk_helper::mesh::Mesh::Mesh(
     std::vector<Vertex> vertices,
     std::vector<uint16_t> indices,
     std::vector<Texture> textures
) : vertices_{vertices}, indices_{indices}, textures_{textures} {
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

    glBindVertexArray(0);
}

void ndk_helper::mesh::Mesh::draw(ndk_helper::shdr::Shader* shader) const {
    unsigned int diffuseNr{0};
    unsigned int specularNr{0};

    for (auto i = 0; i != textures_.size(); ++i) {
        glActiveTexture(GL_TEXTURE0 + i);
        std::string name = textures_[i].type;
        std::string number;
        if (name == "diffuse") {
            number = std::to_string(diffuseNr++);
        } else if (name == "specular") {
            number = std::to_string(specularNr++);
        }
        ndk_helper::shdr::set_int(shader, "material." + name, i);
        glBindTexture(GL_TEXTURE_2D, textures_[i].id);
    }

    glBindVertexArray(vao_);
    glDrawElements(GL_TRIANGLES, indices_.size(), GL_UNSIGNED_SHORT, 0);
    glBindVertexArray(0);
}
