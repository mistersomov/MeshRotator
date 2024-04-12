#ifndef MESHROTATOR_ASSETMANAGER_HPP
#define MESHROTATOR_ASSETMANAGER_HPP

#include "../Mesh.hpp"
#include "../third_party/stb/stb_image.h"
#include <android/asset_manager.h>
#include <memory>

namespace ndk_helper {
    namespace assetmgr {
        class AssetManager {
        public:
            AssetManager(const AssetManager&) = delete;
            AssetManager(AssetManager&&) = delete;
            AssetManager& operator=(const AssetManager&) = delete;
            AssetManager& operator=(AssetManager&&) = delete;
            ~AssetManager();

            const GLuint loadShader(const GLenum type, const std::string& path) const;
            const mesh::Texture loadTexture2D(
                const mesh::TextureType type,
                const std::string& path
            ) const;
            const std::string loadModel(const std::string& path) const;
            const GLuint loadCubeMap(std::vector<std::string> paths) const;
            static AssetManager& instance(AAssetManager* aasetManager);

        private:
            AssetManager(AAssetManager* aasetManager);
            AAssetManager* aasetManager_;

            const GLuint createShaderByType(const GLenum type) const;
            const std::string readAssetFromPath(const std::string& path) const;
            void compileShader(const GLuint shader, const char* source) const;
            bool isShaderCompiled(const GLuint shader) const;
            void handleShaderCompilationError(const GLuint shader) const;
            stbi_uc* generateTextureFromMemory(
                const std::string& buffer,
                int& width,
                int& height,
                int& numberOfChannels
            ) const;
            const GLenum getImageFormat(int& numberOfChannels) const;
            const GLuint createTexture2D(
               GLenum format,
               int& width,
               int& height,
               stbi_uc* data
            ) const;
            void setTextureParameters(GLenum target) const;
        };
    }
}

#endif //MESHROTATOR_ASSETMANAGER_HPP