#ifndef MESH_ROTATOR_MODELMANAGER_HPP
#define MESH_ROTATOR_MODELMANAGER_HPP

#include "AssetManager.hpp"
#include "NDKHelper.hpp"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

namespace mdlmgr {
    class ModelManager {
    public:
        ModelManager(assetmgr::AssetManager* assetManager);

        void load_model(const std::string&);
        std::vector<ndk_helper::mdl::Model> get_models() const;

    private:
        std::unique_ptr<assetmgr::AssetManager> asset_manager_;
        std::vector<ndk_helper::mdl::Model> models_;

        void process_node(aiNode*, const aiScene*);
        ndk_helper::mesh::Mesh process_mesh(aiMesh*, const aiScene*);
        std::vector<ndk_helper::mesh::Texture> process_textures();
    };
}

#endif //MESH_ROTATOR_MODELMANAGER_HPP