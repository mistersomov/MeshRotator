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
            AssetManager(AAssetManager* aasetManager);
            ~AssetManager();

            const GLuint loadShader(const GLenum type, const std::string& path) const;
            const ndk_helper::mesh::Texture loadTexture(
                const std::string& path,
                const std::string& type
            ) const;
            const std::string loadModel(const std::string& path) const;

        private:
            AAssetManager* aasetManager_;

            const GLuint createShaderByType(const GLenum type) const;
            const std::string readAssetFromPath(const std::string& path) const;
            void compileShader(const GLuint shader, const char* source) const;
            bool isShaderCompiled(const GLuint shader) const;
            void handleShaderCompilationError(const GLuint shader) const;
            const GLuint generateTextureFromMemory(const std::string& buffer) const;
            const GLenum getImageFormat(int& numberOfChannels) const;
            const GLuint createTexture(GLenum format, int& width, int& height, stbi_uc* data) const;
            void setTextureParameters() const;
        };
    }
}

#endif //MESHROTATOR_ASSETMANAGER_HPP