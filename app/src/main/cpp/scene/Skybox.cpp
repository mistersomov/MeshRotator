#include "Skybox.hpp"

scene::Skybox::Skybox(
    std::vector<std::string> skyboxFaces,
    Shader* pShader
) : faces_{std::move(skyboxFaces)}, shader_{pShader} {
    vertices_ = {
        1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
        1.0f, 1.0f, -1.0f,
        -1.0f, 1.0f, -1.0f,

        -1.0f, -1.0f, 1.0f,
        1.0f, -1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,
        -1.0f, 1.0f, 1.0f,

        -1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f, 1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, 1.0f,

        1.0f, -1.0f, 1.0f,
        1.0f, -1.0f, -1.0f,
        1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, -1.0f,

        1.0f, -1.0f, 1.0f,
        -1.0f, -1.0f, 1.0f,
        1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,

        1.0f, 1.0f, 1.0f,
        -1.0f, 1.0f, -1.0f,
        1.0f, 1.0f, -1.0f,
        -1.0f, 1.0f, 1.0f,
    };
    indices_ = {
        0, 1, 3,
        2, 0, 3,

        4, 5, 7,
        6, 7, 5,

        8, 9, 11,
        10, 8, 11,

        12, 13, 15,
        14, 12, 15,

        16, 17, 19,
        18, 16, 19,

        20, 21, 23,
        22, 21, 20
    };
}

void scene::Skybox::onCreate(AAssetManager* aAssetManager) {
    glGenVertexArrays(1, &vao_);
    glGenBuffers(1, &vbo_);
    glBindVertexArray(vao_);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_);
    glBufferData(GL_ARRAY_BUFFER, vertices_.size() * sizeof(float), vertices_.data(), GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);

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

void scene::Skybox::onDraw() {
    GLint oldCullFaceMode;
    glGetIntegerv(GL_CULL_FACE_MODE, &oldCullFaceMode);
    GLint oldDepthFuncMode;
    glGetIntegerv(GL_DEPTH_FUNC, &oldDepthFuncMode);
    glCullFace(GL_FRONT);
    glDepthFunc(GL_LEQUAL);
    shader_->activate();
    glBindVertexArray(vao_);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureId_);
    glDrawElements(GL_TRIANGLES, indices_.size(), GL_UNSIGNED_SHORT, 0);
    glBindVertexArray(0);
    glCullFace(oldCullFaceMode);
    glDepthFunc(oldDepthFuncMode);
}
