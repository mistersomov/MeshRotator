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
        void doFrame(Shader* shader);

    private:
        std::vector<std::string> faces_;
        std::vector<GLfloat> vertices_;
        std::vector<uint16_t> indices_;

        GLuint vao_{0};
        GLuint vbo_{0};
        GLuint ebo_{0};
        GLuint textureId_{0};
    };
}

#endif //MESH_ROTATOR_SKYBOX_HPP
