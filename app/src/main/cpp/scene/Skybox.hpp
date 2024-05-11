#ifndef MESH_ROTATOR_SKYBOX_HPP
#define MESH_ROTATOR_SKYBOX_HPP

#include "NDKHelper.hpp"

namespace scene {
    using Shader = ndk_helper::shdr::Shader;
    using AssetManager = ndk_helper::assetmgr::AssetManager;

    class Skybox {
    public:
        Skybox(std::vector<std::string> skyboxFaces);

        void onCreate(AAssetManager* aAssetManager);
        void onDraw(Shader* shader);

    private:
        std::vector<std::string> faces_;
        std::vector<GLfloat> vertices_;
        std::vector<uint16_t> indices_;

        GLuint vao_ = -1;
        GLuint vbo_ = -1;
        GLuint ebo_ = -1;
        GLuint textureId_ = -1;
    };
}

#endif //MESH_ROTATOR_SKYBOX_HPP
