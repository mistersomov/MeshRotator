#include "ModelManager.hpp"
#include "../utils/AndroidOut.h"

#include <exception>

mdlmgr::ModelManager::ModelManager(
    ndk_helper::assetmgr::AssetManager& assetManager
) : assetManager_{assetManager} {}

void mdlmgr::ModelManager::loadModelFromPath(const std::string& path) {
    auto modelAsset = assetManager_.loadModel(path);
    Assimp::Importer importer;
    auto scene = importer.ReadFileFromMemory(
        modelAsset.data(),
        modelAsset.length(),
        aiProcess_Triangulate
            | aiProcess_JoinIdenticalVertices
            | aiProcess_CalcTangentSpace
            | aiProcess_GenSmoothNormals
    );
    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        aout << "ERROR::ASSIMP::" << importer.GetErrorString() << std::endl;
        throw importer.GetException();
    }
    process_node(scene->mRootNode, scene);
}

std::vector<ndk_helper::mdl::Model> mdlmgr::ModelManager::get_models() const {
    return models_;
}

void mdlmgr::ModelManager::process_node(aiNode* node, const aiScene* scene) {
    for (auto i = 0; i != node->mNumMeshes; ++i) {
        auto mesh = scene->mMeshes[node->mMeshes[i]];
        models_.push_back(process_mesh(mesh, scene));
    }
    for (auto i = 0; i != node->mNumChildren; ++i) {
        process_node(node->mChildren[i], scene);
    }
}

ndk_helper::mesh::Mesh mdlmgr::ModelManager::process_mesh(aiMesh* mesh, const aiScene* scene) {
    std::vector<ndk_helper::mesh::Vertex> vertices;
    std::vector<uint16_t> indices;

    for (auto i = 0; i != mesh->mNumVertices; ++i) {
        ndk_helper::mesh::Vertex vertex;
        glm::vec3 data;

        data.x = mesh->mVertices[i].x;
        data.y = mesh->mVertices[i].y;
        data.z = mesh->mVertices[i].z;
        vertex.position = data;

        if (mesh->HasNormals()) {
            data.x = mesh->mNormals[i].x;
            data.y = mesh->mNormals[i].y;
            data.z = mesh->mNormals[i].z;
            vertex.normal = data;
        }
        if (mesh->mTextureCoords[0]) {
            glm::vec2 vec;
            vec.x = mesh->mTextureCoords[0][i].x;
            vec.y = mesh->mTextureCoords[0][i].y;
            vertex.texCoord = vec;
            // tangent
            if (mesh->mTangents) {
                data.x = mesh->mTangents[i].x;
                data.y = mesh->mTangents[i].y;
                data.z = mesh->mTangents[i].z;
                vertex.tangent = data;
            }
            // bitangent
            if (mesh->mBitangents) {
                data.x = mesh->mBitangents[i].x;
                data.y = mesh->mBitangents[i].y;
                data.z = mesh->mBitangents[i].z;
                vertex.bitangent = data;
            }
        } else {
            vertex.texCoord = glm::vec2(0.0f, 0.0f);
        }
        vertices.emplace_back(vertex);
    }
    for (auto i = 0; i != mesh->mNumFaces; ++i) {
        aiFace face = mesh->mFaces[i];

        for (auto j = 0; j < face.mNumIndices; j++)
            indices.emplace_back(face.mIndices[j]);
    }

    return {vertices, indices, process_textures()};
}

std::vector<ndk_helper::mesh::Texture> mdlmgr::ModelManager::process_textures() {
    std::vector<ndk_helper::mesh::Texture> textures;
    auto diffuse =
            assetManager_.loadTexture("model/pillar/pillar_1_BaseColor.png", "diffuse");
    textures.emplace_back(diffuse);

    auto normal =
            assetManager_.loadTexture("model/pillar/pillar_1_NormaL_GL.png", "normal");
    textures.emplace_back(normal);

    return textures;
}