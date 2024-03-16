#ifndef MESH_ROTATOR_MODELMANAGER_HPP
#define MESH_ROTATOR_MODELMANAGER_HPP

#include "NDKHelper.hpp"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

namespace mdlmgr {
    class ModelManager {
    public:
        ModelManager(ndk_helper::assetmgr::AssetManager& assetManager);

        void loadModelFromPath(const std::string& path);
        std::vector<ndk_helper::mdl::Model_Impl> get_models() const;
        void translate(glm::vec3 newPosition);
        void scale(glm::vec3 newScale);

    private:
        ndk_helper::assetmgr::AssetManager assetManager_;
        std::vector<ndk_helper::mdl::Model_Impl> models_;

        void process_node(aiNode*, const aiScene*);
        ndk_helper::mesh::Mesh process_mesh(aiMesh*);
        std::vector<ndk_helper::mesh::Texture> process_textures();
    };
}

#endif //MESH_ROTATOR_MODELMANAGER_HPP