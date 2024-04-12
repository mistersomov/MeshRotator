#include "ModelManager.hpp"
#include "../utils/AndroidOut.hpp"
#include <exception>

ndk_helper::mdlmgr::ModelManager::ModelManager(AssetManager& assetManager) : assetManager_{assetManager} {}

ndk_helper::mdlmgr::ModelManager& ndk_helper::mdlmgr::ModelManager::instance(AssetManager &assetManager) {
    static ModelManager modelManager{assetManager};
    return modelManager;
}

ndk_helper::mdl::Model ndk_helper::mdlmgr::ModelManager::getModelFromPath(const std::string& path) {
    auto modelAsset = assetManager_.loadModel(path);
    Assimp::Importer importer;
    auto scene = importer.ReadFileFromMemory(
        modelAsset.data(),
        modelAsset.length(),
        aiProcess_Triangulate
            | aiProcess_JoinIdenticalVertices
            | aiProcess_CalcTangentSpace
            | aiProcess_GenSmoothNormals
            | aiProcess_FlipUVs
    );
    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        aout << "ERROR::ASSIMP::" << importer.GetErrorString() << std::endl;
        throw importer.GetException();
    }
    mdl::Model model;
    process_node(model, scene->mRootNode, scene);

    return model;
}

void ndk_helper::mdlmgr::ModelManager::applyTextures(
    ndk_helper::mdl::Model& model,
    std::map<mesh::TextureType, std::string> textures
) {
    std::for_each(
        textures.begin(),
        textures.end(),
        [&](const std::pair<mesh::TextureType, std::string&> texture
    ) {
        model.addTexture(assetManager_.loadTexture2D(texture.first, texture.second));
    });
}

void ndk_helper::mdlmgr::ModelManager::process_node(
    mdl::Model& model,
    aiNode* node,
    const aiScene* scene
) {
    for (auto i = 0; i != node->mNumMeshes; ++i) {
        auto mesh = scene->mMeshes[node->mMeshes[i]];
        auto processedMesh = process_mesh(mesh);

        model.addMesh(processedMesh);
    }
    for (auto i = 0; i != node->mNumChildren; ++i) {
        process_node(model,node->mChildren[i], scene);
    }
}

ndk_helper::mesh::Mesh ndk_helper::mdlmgr::ModelManager::process_mesh(aiMesh* mesh) {
    std::vector<ndk_helper::mesh::Vertex> vertices;
    std::vector<uint16_t> indices;

    for (auto i = 0; i != mesh->mNumVertices; ++i) {
        ndk_helper::mesh::Vertex vertex;

        vertex.position = applyPosition(mesh->mVertices[i]);
        if (mesh->HasNormals()) {
            vertex.normal = applyNormals(mesh->mNormals[i]);
        }
        if (mesh->mTextureCoords[0]) {
            vertex.texCoord = applyTextureCoords(mesh->mTextureCoords[0][i]);
            if (mesh->mTangents) {
                vertex.tangent = applyTangents(mesh->mTangents[i]);
            }
            if (mesh->mBitangents) {
                vertex.bitangent = applyBitangents(mesh->mBitangents[i]);
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

    return {vertices, indices};
}

glm::vec3 ndk_helper::mdlmgr::ModelManager::applyPosition(aiVector3D position) {
    return glm::vec3{position.x, position.y, position.z};
}

glm::vec3 ndk_helper::mdlmgr::ModelManager::applyNormals(aiVector3D normal) {
    return glm::vec3{normal.x, normal.y, normal.z};
}

glm::vec2 ndk_helper::mdlmgr::ModelManager::applyTextureCoords(aiVector3D coordinates) {
    return glm::vec2{coordinates.x, coordinates.y};
}

glm::vec3 ndk_helper::mdlmgr::ModelManager::applyTangents(aiVector3D tangents) {
    return glm::vec3{tangents.x, tangents.y, tangents.z};
}

glm::vec3 ndk_helper::mdlmgr::ModelManager::applyBitangents(aiVector3D bitangents) {
    return glm::vec3{bitangents.x, bitangents.y, bitangents.z};
}
