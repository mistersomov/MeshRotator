#ifndef MESHROTATOR_RENDERER_HPP
#define MESHROTATOR_RENDERER_HPP

#include "NDKHelper.hpp"
#include "manager/AssetManager.hpp"
#include "utils/AndroidOut.h"

#include <EGL/egl.h>
#include <game-activity/native_app_glue/android_native_app_glue.h>

namespace rndr {
    class Renderer {
        std::unique_ptr<android_app> app_;
        std::unique_ptr<assetmgr::AssetManager> asset_manager_;
        std::unique_ptr<ndk_helper::timemgr::TimeManager> time_manager_;

        EGLDisplay display_{EGL_NO_DISPLAY};
        EGLSurface surface_{EGL_NO_SURFACE};
        EGLContext context_{EGL_NO_CONTEXT};
        EGLint width_{0};
        EGLint height_{0};

        std::vector<ndk_helper::mdl::Model> models_;
        std::unique_ptr<ndk_helper::shdr::Shader> shader_;
        std::unique_ptr<ndk_helper::shdr::Shader> outlined_;
        std::unique_ptr<ndk_helper::shdr::Shader> normalShader_;

        GLuint uboMatrices_{0};

        void prepare_graphics();
        void create_model();
        void create_matrix_uniform_buffer();
    public:
        inline Renderer(
            android_app *app
        ) : app_{app},
            asset_manager_{new assetmgr::AssetManager(app->activity->assetManager)},
            time_manager_{new ndk_helper::timemgr::TimeManager()}
        {
            prepare_graphics();
            create_model();
            create_matrix_uniform_buffer();
        };
        ~Renderer();

        void render();
        void handle_input();
    };
}

#endif //MESHROTATOR_RENDERER_HPP
