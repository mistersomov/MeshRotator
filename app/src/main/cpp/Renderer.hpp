#ifndef MESHROTATOR_RENDERER_HPP
#define MESHROTATOR_RENDERER_HPP

#include <EGL/egl.h>
#include <game-activity/native_app_glue/android_native_app_glue.h>
#include "scene/MainScene.hpp"

namespace rndr {
    class Renderer {
        std::unique_ptr<android_app> app_;
        std::unique_ptr<ndk_helper::timemgr::TimeManager> timeManager_;

        EGLDisplay display_{EGL_NO_DISPLAY};
        EGLSurface surface_{EGL_NO_SURFACE};
        EGLContext context_{EGL_NO_CONTEXT};
        EGLint width_{0};
        EGLint height_{0};

        std::unique_ptr<scene::Scene> scene_;

        void prepare_graphics();
    public:
        Renderer(android_app *app);
        ~Renderer();

        void render();
        void handle_input();
    };
}

#endif //MESHROTATOR_RENDERER_HPP
