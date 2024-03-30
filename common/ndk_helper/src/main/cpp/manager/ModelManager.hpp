#ifndef MESH_ROTATOR_MODELMANAGER_HPP
#define MESH_ROTATOR_MODELMANAGER_HPP

#include "AssetManager.hpp"
#include "../Model.hpp"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <map>

namespace ndk_helper {
    namespace mdlmgr {
        using AssetManager = assetmgr::AssetManager;
        class ModelManager {
        public:
            ModelManager(const ModelManager&) = delete;
            ModelManager(ModelManager&&) = delete;
            ModelManager& operator=(const ModelManager&) = delete;
            ModelManager& operator=(ModelManager&&) = delete;

            static ModelManager& instance(AssetManager &assetManager);
            mdl::Model getModelFromPath(const std::string& path);
            void applyTextures(
                ndk_helper::mdl::Model& model,
                std::map<mesh::TextureType, std::string> textures
            );

        private:
            ModelManager(AssetManager &assetManager);

            void process_node(mdl::Model& model, aiNode* node, const aiScene* scene);
            mesh::Mesh process_mesh(aiMesh* mesh);
            glm::vec3 applyPosition(aiVector3D position);
            glm::vec3 applyNormals(aiVector3D normal);
            glm::vec2 applyTextureCoords(aiVector3D coordinates);
            glm::vec3 applyTangents(aiVector3D tangents);
            glm::vec3 applyBitangents(aiVector3D bitangents);

            AssetManager& assetManager_;
        };
    }
}

#endif //MESH_ROTATOR_MODELMANAGER_HPP