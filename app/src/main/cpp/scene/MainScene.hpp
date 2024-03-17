#ifndef MESHROTATOR_MAINSCENE_HPP
#define MESHROTATOR_MAINSCENE_HPP

#include "Scene.hpp"
#include "NDKHelper.hpp"

namespace scene {
    using AssetManager = ndk_helper::assetmgr::AssetManager;
    using TimeManager = ndk_helper::timemgr::TimeManager;
    using ModelManager = ndk_helper::mdlmgr::ModelManager;
    using Model = ndk_helper::mdl::Model;
    using ShaderManager = ndk_helper::shdrmgr::ShaderManager;
    using Shader = ndk_helper::shdr::Shader;

    class MainScene : public Scene {
    public:
        MainScene(
            float width,
            float height,
            AAssetManager* aAssetManager,
            TimeManager& timeManager
        );
        ~MainScene() override;

        void onCreate() override;
        void doFrame() override;

    private:
        AAssetManager* aAssetManager_;
        TimeManager timeManager_;

        std::vector<Model> models_;

        std::unique_ptr<Shader> pillarShader_;
        std::unique_ptr<Shader> outlineShader_;
        std::unique_ptr<Shader> normalVectorShader_;
        std::unique_ptr<Shader> screenShader_;

        glm::vec3 viewPos_;

        GLuint uboMatrices_;

        void renderOutline(Model& model) const;
        void renderNormalVector(Model& model) const;
        void renderFramebuffer() const;
        void initShaders();
        void initUniformBuffers();
        void destroyUniformBuffers();
        void initLights();
        void addPointLights() const;
        void addDirectionalLights() const;
        void initModels();
        void addPillar();
        void addBrokenPillar();
    };
}

#endif //MESHROTATOR_MAINSCENE_HPP
