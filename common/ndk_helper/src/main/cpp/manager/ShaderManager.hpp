#ifndef MESH_ROTATOR_SHADERMANAGER_HPP
#define MESH_ROTATOR_SHADERMANAGER_HPP

#include "../Shader.hpp"
#include "AssetManager.hpp"

namespace ndk_helper {
    namespace shdrmgr {
        using AssetManager = assetmgr::AssetManager;
        using Shader = shdr::Shader;

        class ShaderManager {
        public:
            ShaderManager(const ShaderManager&) = delete;
            ShaderManager(ShaderManager&&) = delete;
            ShaderManager& operator=(const ShaderManager&) = delete;
            ShaderManager& operator=(ShaderManager&&) = delete;

            static ShaderManager& instance(AssetManager& assetManager);
            std::unique_ptr<Shader> getShader(
                const std::string& vSource,
                const std::string& fSource,
                const std::string& gSource = ""
            );

        private:
            ShaderManager(AssetManager& assetManager);
            AssetManager& assetManager_;
        };
    }
}

#endif //MESH_ROTATOR_SHADERMANAGER_HPP
