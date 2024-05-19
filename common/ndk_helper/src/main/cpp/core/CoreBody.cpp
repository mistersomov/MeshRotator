#include "CoreBody.hpp"
#include "../utils/AndroidOut.hpp"
#include "../utils/Utility.hpp"
#include "../utils/AssimpUtility.hpp"
#include <algorithm>
#include <exception>

using AssetManager = ndk_helper::assetmgr::AssetManager;

ndk_helper::core::CoreBody::CoreBody(
    const std::string& path,
    AAssetManager* pAssetManager
) : assetManager_{pAssetManager} {
    buffers_.resize(BUFFERS_COUNT);
    try {
        loadModel(path);
    } catch (const std::exception& e) {
        aout << "ERROR::COREBODY::" << e.what() << std::endl;
    }
}

ndk_helper::core::CoreBody::~CoreBody() {
    if (pScene_ != nullptr) {
        delete pScene_;
        pScene_ = nullptr;
    }
    glDisableVertexAttribArray(POSITION);
    glDisableVertexAttribArray(NORMAL);
    glDisableVertexAttribArray(TEX_COORD);
    glDisableVertexAttribArray(TANGENT);
    glDisableVertexAttribArray(BI_TANGENT);
    glDeleteBuffers(BUFFERS_COUNT, buffers_.data());
    glDeleteVertexArrays(1, &vao_);
    vao_ = -1;
}

void ndk_helper::core::CoreBody::loadModel(const std::string& path) {
    AssetManager& assetManager = AssetManager::instance(assetManager_);
    auto modelAsset = assetManager.loadModel(path);
    pScene_ = importer.ReadFileFromMemory(
        modelAsset.data(),
        modelAsset.length(),
        aiProcess_Triangulate
            | aiProcess_JoinIdenticalVertices
            | aiProcess_CalcTangentSpace
            | aiProcess_GenSmoothNormals
            | aiProcess_FlipUVs
            | aiProcess_OptimizeGraph
    );

    if (!pScene_ || pScene_->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !pScene_->mRootNode) {
        aout << "ERROR::ASSIMP::" << importer.GetErrorString() << std::endl;
        throw importer.GetException();
    } else {
        glGenVertexArrays(1, &vao_);
        glBindVertexArray(vao_);
        glGenBuffers(BUFFERS_COUNT, buffers_.data());
        initGeometry();
        glBindVertexArray(0);
    }
}

void ndk_helper::core::CoreBody::initGeometry() {
    countVerticesAndIndices();
    reserveSpace();
    initMeshes();
    fillBuffers();
}

void ndk_helper::core::CoreBody::countVerticesAndIndices() {
    meshes_.resize(pScene_->mNumMeshes);
    for (uint32_t i = 0; i != meshes_.size(); ++i) {
        meshes_[i].numIndices = pScene_->mMeshes[i]->mNumFaces * 3;
        meshes_[i].baseVertex = numVertices_;
        meshes_[i].baseIndex = numIndices_;

        numVertices_ += pScene_->mMeshes[i]->mNumVertices;
        numIndices_ += meshes_[i].numIndices;
    }
}

void ndk_helper::core::CoreBody::reserveSpace() {
    positions_.reserve(numVertices_);
    normals_.reserve(numVertices_);
    texCoords_.reserve(numVertices_);
    tangents_.reserve(numVertices_);
    biTangents_.reserve(numVertices_);
    indices_.reserve(numIndices_);
}

void ndk_helper::core::CoreBody::initMeshes() {
    for (uint32_t i = 0; i != meshes_.size(); ++i) {
        const aiMesh* pMesh = pScene_->mMeshes[i];
        initSingleMesh(pMesh);
    }
}

void ndk_helper::core::CoreBody::initSingleMesh(const aiMesh* pMesh) {
    for (uint32_t i = 0; i != pMesh->mNumVertices; ++i) {
        positions_.push_back(glm::vec3(pMesh->mVertices[i].x, pMesh->mVertices[i].y, pMesh->mVertices[i].z));
        if (pMesh->HasNormals()) {
            normals_.push_back(glm::vec3(pMesh->mNormals[i].x, pMesh->mNormals[i].y, pMesh->mNormals[i].z));
        } else {
            normals_.push_back(glm::vec3(0.0f, 1.0f, 0.0f));
        }
        if (pMesh->mTextureCoords[0]) {
            texCoords_.push_back(glm::vec2(pMesh->mTextureCoords[0][i].x, pMesh->mTextureCoords[0][i].y));
            if (pMesh->mTangents) {
                tangents_.push_back(glm::vec3(pMesh->mTangents[i].x, pMesh->mTangents[i].y, pMesh->mTangents[i].z));
            }
            if (pMesh->mBitangents) {
                biTangents_.push_back(glm::vec3(pMesh->mBitangents[i].x, pMesh->mBitangents[i].y, pMesh->mBitangents[i].z));
            }
        } else {
            texCoords_.push_back(glm::vec2(0.0f));
            tangents_.push_back(glm::vec3(0.0f));
            biTangents_.push_back(glm::vec3(0.0f));
        }
    }
    for (uint32_t i = 0; i != pMesh->mNumFaces; ++i) {
        const aiFace& face = pMesh->mFaces[i];

        indices_.push_back(face.mIndices[0]);
        indices_.push_back(face.mIndices[1]);
        indices_.push_back(face.mIndices[2]);
    }
}

void ndk_helper::core::CoreBody::fillBuffers() {
    fillPositionBuffer();
    fillNormalBuffer();
    fillTexCoordBuffer();
    fillTangentBuffer();
    fillBiTangentBuffer();
    fillIndexBuffer();
}

void ndk_helper::core::CoreBody::fillPositionBuffer() {
    glBindBuffer(GL_ARRAY_BUFFER, buffers_[POSITION]);
    glBufferData(
        GL_ARRAY_BUFFER,
        positions_.size() * sizeof(positions_[0]),
        positions_.data(),
        GL_STATIC_DRAW
    );
    glEnableVertexAttribArray(POSITION);
    glVertexAttribPointer(
        POSITION,
        3,
        GL_FLOAT,
        GL_FALSE,
        0,
        nullptr
    );
}

void ndk_helper::core::CoreBody::fillNormalBuffer() {
    glBindBuffer(GL_ARRAY_BUFFER, buffers_[NORMAL]);
    glBufferData(
        GL_ARRAY_BUFFER,
        normals_.size() * sizeof(normals_[0]),
        normals_.data(),
        GL_STATIC_DRAW
    );
    glEnableVertexAttribArray(NORMAL);
    glVertexAttribPointer(
        NORMAL,
        3,
        GL_FLOAT,
        GL_FALSE,
        0,
        nullptr
    );
}

void ndk_helper::core::CoreBody::fillTexCoordBuffer() {
    glBindBuffer(GL_ARRAY_BUFFER, buffers_[TEX_COORD]);
    glBufferData(
        GL_ARRAY_BUFFER,
        texCoords_.size() * sizeof(texCoords_[0]),
        texCoords_.data(),
        GL_STATIC_DRAW
    );
    glEnableVertexAttribArray(TEX_COORD);
    glVertexAttribPointer(
        TEX_COORD,
        2,
        GL_FLOAT,
        GL_FALSE,
        0,
        nullptr
    );
}

void ndk_helper::core::CoreBody::fillTangentBuffer() {
    glBindBuffer(GL_ARRAY_BUFFER, buffers_[TANGENT]);
    glBufferData(
        GL_ARRAY_BUFFER,
        tangents_.size() * sizeof(tangents_[0]),
        tangents_.data(),
        GL_STATIC_DRAW
    );
    glEnableVertexAttribArray(TANGENT);
    glVertexAttribPointer(
        TANGENT,
        3,
        GL_FLOAT,
        GL_FALSE,
        0,
        nullptr
    );
}

void ndk_helper::core::CoreBody::fillBiTangentBuffer() {
    glBindBuffer(GL_ARRAY_BUFFER, buffers_[BI_TANGENT]);
    glBufferData(
        GL_ARRAY_BUFFER,
        biTangents_.size() * sizeof(biTangents_[0]),
        biTangents_.data(),
        GL_STATIC_DRAW
    );
    glEnableVertexAttribArray(BI_TANGENT);
    glVertexAttribPointer(
        BI_TANGENT,
        3,
        GL_FLOAT,
        GL_FALSE,
        0,
        nullptr
    );
}

void ndk_helper::core::CoreBody::fillIndexBuffer() {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffers_[INDEX]);
    glBufferData(
        GL_ELEMENT_ARRAY_BUFFER,
        indices_.size() * sizeof(indices_[0]),
        indices_.data(),
        GL_STATIC_DRAW
    );
}

void ndk_helper::core::CoreBody::addTextures(std::map<TextureType, std::string> textures) {
    AssetManager& assetManager = AssetManager::instance(assetManager_);
    std::for_each(
        textures.begin(),
        textures.end(),
        [&](const std::pair<TextureType, std::string&> texture
    ) {
        auto texture_ = assetManager.loadTexture2D(texture.first, texture.second);
        textures_.push_back(texture_);
    });
}

void ndk_helper::core::CoreBody::onDraw() const {
    glBindVertexArray(vao_);
    std::for_each(meshes_.begin(), meshes_.end(), [&](const Mesh& mesh) {
        glDrawElementsBaseVertex(
            GL_TRIANGLES,
            mesh.numIndices,
            GL_UNSIGNED_SHORT,
            (void*) (sizeof(GLushort) * mesh.baseIndex),
            mesh.baseVertex
        );
    });
    glBindVertexArray(0);
}
