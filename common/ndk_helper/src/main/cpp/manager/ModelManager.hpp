#ifndef MESH_ROTATOR_MODELMANAGER_HPP
#define MESH_ROTATOR_MODELMANAGER_HPP

#include "AssetManager.hpp"
#include "../Model.hpp"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

namespace ndk_helper {
    namespace mdlmgr {
        class ModelManager {
        public:
            ModelManager(assetmgr::AssetManager &assetManager);

            void loadModelFromPath(const std::string &path);
            std::vector<mdl::Model> get_models() const;
            void translate(glm::vec3 newPosition);
            void scale(glm::vec3 newScale);

        private:
            std::unique_ptr<assetmgr::AssetManager> assetManager_;
            std::vector<mdl::Model> models_;

            void process_node(aiNode* node, const aiScene* scene);
            mesh::Mesh process_mesh(aiMesh* mesh);
            std::vector<mesh::Texture> process_textures();
            glm::vec3 applyPosition(aiVector3D position);
            glm::vec3 applyNormals(aiVector3D normal);
            glm::vec2 applyTextureCoords(aiVector3D coordinates);
            glm::vec3 applyTangents(aiVector3D tangents);
            glm::vec3 applyBitangents(aiVector3D bitangents);
        };
    }
}

#endif //MESH_ROTATOR_MODELMANAGER_HPP