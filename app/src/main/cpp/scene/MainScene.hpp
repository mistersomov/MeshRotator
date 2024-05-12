#ifndef MESHROTATOR_MAINSCENE_HPP
#define MESHROTATOR_MAINSCENE_HPP

#include "Scene.hpp"
#include "Skybox.hpp"
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

    protected:
        void initShaders() override;
        void initUniformBuffers() override;
        void initModels() override;
        void initLights() override;
        void initSkybox();

    private:
        AAssetManager* aAssetManager_;
        TimeManager timeManager_;

        std::vector<Model> models_;

        std::unique_ptr<Shader> pillarShader_;
        std::unique_ptr<Shader> outlineShader_;
        std::unique_ptr<Shader> screenShader_;
        std::unique_ptr<Shader> skyboxShader_;

        std::unique_ptr<Skybox> skybox_;

        glm::vec3 viewPos_;
        glm::mat4 identityMat_ = glm::mat4(1.0f);

        GLuint uboMatrices_;

        //TODO Think about SOLID here
        void renderModels(Model& model) const;
        void renderFramebuffer() const;
        void destroyUniformBuffers();
        void addPillar();
        void addBrokenPillar();
        void addAircraft();
        void addPointLights() const;
        void addDirectionalLights() const;
        void renderOutline(Model& model) const;
    };
}

#endif //MESHROTATOR_MAINSCENE_HPP
