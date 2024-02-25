#ifndef MESHROTATOR_ASSETMANAGER_HPP
#define MESHROTATOR_ASSETMANAGER_HPP

#include "NDKHelper.hpp"

#include <android/asset_manager.h>
#include <memory>

namespace assetmgr {
    class AssetManager {
    public:
        AssetManager(AAssetManager* assetManager);
        ~AssetManager();

        ndk_helper::shdr::Shader* load_shader(
            const std::string&,
            const std::string&,
            const std::string&
        );
        std::shared_ptr<ndk_helper::mesh::Texture> load_texture(
            const std::string&,
            const std::string&
        );
        const std::string load_model(const std::string&) const;
    private:
        AAssetManager* assetManager_;

        GLuint loadShader(GLenum, const std::string&);
    };
}

#endif //MESHROTATOR_ASSETMANAGER_HPP