#include "Skybox.hpp"

scene::Skybox::Skybox(std::vector<std::string> skyboxFaces) : faces_{std::move(skyboxFaces)} {
    vertices_ = {
        -1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,
        1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        1.0f, -1.0f,  1.0f,
        1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f,  1.0f,

        1.0f, -1.0f,  1.0f,
        1.0f, -1.0f, -1.0f,
        1.0f,  1.0f, -1.0f,
        1.0f,  1.0f,  1.0f,

        -1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
        1.0f, -1.0f,  1.0f,
        1.0f, -1.0f, -1.0f,

        -1.0f,  1.0f,  1.0f,
        1.0f,  1.0f,  1.0f,
        1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f
    };
    indices_ = {
        0, 1, 3,
        2, 3, 1,

        4, 5, 7,
        6, 7, 5,

        8, 9, 11,
        10, 11, 9,

        12, 13, 15,
        14, 15, 13,

        16, 17, 18,
        16, 18, 19,

        20, 21, 23,
        22, 23, 21
    };
}

void scene::Skybox::onCreate(AAssetManager* aAssetManager) {
    glGenVertexArrays(1, &vao_);
    glGenBuffers(1, &vbo_);
    glBindVertexArray(vao_);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_);
    glBufferData(GL_ARRAY_BUFFER, vertices_.size() * sizeof(float), vertices_.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
    glEnableVertexAttribArray(0);

    glGenBuffers(1, &ebo_);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo_);
    glBufferData(
        GL_ELEMENT_ARRAY_BUFFER,
        indices_.size() * sizeof(uint16_t),
        indices_.data(),
        GL_STATIC_DRAW
    );
    glBindVertexArray(0);

    AssetManager& assetManager = AssetManager::instance(aAssetManager);
    textureId_ = assetManager.loadCubeMap(faces_);
}

void scene::Skybox::doFrame(Shader* shader) {
    glDepthMask(GL_FALSE);
    glDepthFunc(GL_LEQUAL);
    shader->activate();
    glBindVertexArray(vao_);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureId_);
    glDrawElements(GL_TRIANGLES, indices_.size(), GL_UNSIGNED_SHORT, 0);
    glBindVertexArray(0);
    glDepthMask(GL_TRUE);
    glDepthFunc(GL_LESS);
}
