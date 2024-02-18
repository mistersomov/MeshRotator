#ifndef MESHROTATOR_ASSETMANAGER_HPP
#define MESHROTATOR_ASSETMANAGER_HPP

#include "NDKHelper.hpp"

#include <android/asset_manager.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <memory>

namespace assetmgr {
    class AssetManager {
        AAssetManager* assetManager_;

        GLuint loadShader(GLenum, const std::string&);
        void process_node(std::vector<aiMesh*>&, aiNode*, const aiScene*);
    public:
        AssetManager(AAssetManager* assetManager);
        ~AssetManager();

        ndk_helper::shdr::Shader* load_shader(
            const std::string&,
            const std::string&,
            const std::string&
        );
        std::shared_ptr<ndk_helper::mesh::Texture> load_texture(const std::string&, const std::string&);
        std::vector<aiMesh*> load_mesh(const std::string&);
    };
}

#endif //MESHROTATOR_ASSETMANAGER_HPP
