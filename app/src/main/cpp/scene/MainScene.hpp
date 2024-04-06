#ifndef MESHROTATOR_MAINSCENE_HPP
#define MESHROTATOR_MAINSCENE_HPP

#include "Scene.hpp"
#include "../model/aircraft/Aircraft.hpp"
#include "Skybox.hpp"
#include "NDKHelper.hpp"

namespace scene {
    using AssetManager = ndk_helper::assetmgr::AssetManager;
    using TimeManager = ndk_helper::timemgr::TimeManager;
    using CoreModel = ndk_helper::core::CoreBody;
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

    private:
        AAssetManager* aAssetManager_;
        TimeManager timeManager_;

        std::unique_ptr<Shader> aircraftShader_;
        std::unique_ptr<Shader> outlineShader_;
        std::unique_ptr<Shader> screenShader_;
        std::unique_ptr<Shader> skyboxShader_;

        std::unique_ptr<model::Aircraft> aircraft_;
        std::unique_ptr<CoreModel> obstacle_;
        std::unique_ptr<Skybox> skybox_;

        glm::vec3 viewPos_;

        GLuint uboMatrices_;

        void onDrawFramebuffer() const;
        void destroyUniformBuffers();
        void addAircraft();
        void addPointLights() const;
        void addDirectionalLights() const;
        void renderOutline(CoreModel& model) const;
    };
}

#endif //MESHROTATOR_MAINSCENE_HPP
