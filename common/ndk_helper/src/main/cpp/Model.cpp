#include "Model.hpp"
#include "../../../../../app/src/main/cpp/utils/AndroidOut.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <string>

ndk_helper::mdl::Model::Model(const std::vector<aiMesh*>& meshes) {
//    for(auto mesh : meshes) {
//        process_mesh(mesh);
//    }
    process_mesh(meshes[0]);
}

ndk_helper::mdl::Model::Model(const std::vector<GLfloat>& vertices, const std::vector<uint16_t>& indices) {
    std::vector<ndk_helper::mesh::Vertex> meshVertices;

    for (int i = 0; i != vertices.size(); i += 8) {
        ndk_helper::mesh::Vertex vert;
        vert.position = glm::vec3(vertices[i], vertices[i + 1], vertices[i + 2]);
        vert.normal = glm::vec3(vertices[i + 3], vertices[i + 4], vertices[i + 5]);
        vert.texCoord = glm::vec2(vertices[i + 6], vertices[i + 7]);
        meshVertices.push_back(vert);
    }
    meshes_.push_back(ndk_helper::mesh::Mesh(meshVertices, indices));
}

ndk_helper::mdl::Model::Model(
    const std::vector<GLfloat>& vertices,
    const std::vector<uint16_t>& indices,
    const std::vector<ndk_helper::mesh::Texture>& textures
) {
    std::vector<ndk_helper::mesh::Vertex> meshVertices;

    for (int i = 0; i != vertices.size(); i += 8) {
        ndk_helper::mesh::Vertex vert;
        vert.position = glm::vec3(vertices[i], vertices[i + 1], vertices[i + 2]);
        vert.normal = glm::vec3(vertices[i + 3], vertices[i + 4], vertices[i + 5]);
        vert.texCoord = glm::vec2(vertices[i + 6], vertices[i + 7]);
        meshVertices.push_back(vert);
    }
    meshes_.push_back(ndk_helper::mesh::Mesh(meshVertices, indices, textures));
}

void ndk_helper::mdl::Model::draw(ndk_helper::shdr::Shader &shader) const {
    for (auto i = meshes_.begin(), end = meshes_.end(); i != end; ++i) {
        i->draw(&shader);
    }
}

void ndk_helper::mdl::Model::process_mesh(aiMesh* mesh) {
    std::vector<ndk_helper::mesh::Vertex> vertices;
    std::vector<uint16_t> indices;
    std::vector<ndk_helper::mesh::Texture> textures{};

    for (auto i = 0; i != mesh->mNumVertices; i++) {
        ndk_helper::mesh::Vertex vertex;
        glm::vec3 vector;

        vector.x = mesh->mVertices[i].x;
        vector.y = mesh->mVertices[i].y;
        vector.z = mesh->mVertices[i].z;
        vertex.position = vector;

        if (mesh->HasNormals()) {
            vector.x = mesh->mNormals[i].x;
            vector.y = mesh->mNormals[i].y;
            vector.z = mesh->mNormals[i].z;
            vertex.normal = vector;
        }
        if (mesh->mTextureCoords[0]) {
            glm::vec2 vec;
            vec.x = mesh->mTextureCoords[0][i].x;
            vec.y = mesh->mTextureCoords[0][i].y;
            vertex.texCoord = vec;
            // tangent
//            if (mesh->mTangents) {
//                vector.x = mesh->mTangents[i].x;
//                vector.y = mesh->mTangents[i].y;
//                vector.z = mesh->mTangents[i].z;
//                vertex.tangent = vector;
//            }
//            // bitangent
//            if (mesh->mBitangents) {
//                vector.x = mesh->mBitangents[i].x;
//                vector.y = mesh->mBitangents[i].y;
//                vector.z = mesh->mBitangents[i].z;
//                vertex.bitangent = vector;
//            }
        } else {
            vertex.texCoord = glm::vec2(0.0f, 0.0f);
        }

        vertices.emplace_back(vertex);
    }
    for (auto i = 0; i != mesh->mNumFaces; i++) {
        auto face = mesh->mFaces[i];
        for (auto j = 0; j != face.mNumIndices; j++) {
            indices.emplace_back(face.mIndices[j]);
        }
    }
    meshes_.emplace_back(ndk_helper::mesh::Mesh{vertices, indices, textures});
}
