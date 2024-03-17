#ifndef MESHROTATOR_RENDERER_HPP
#define MESHROTATOR_RENDERER_HPP

#include "NDKHelper.hpp"
#include <EGL/egl.h>
#include <game-activity/native_app_glue/android_native_app_glue.h>

namespace rndr {
    class Renderer {
        std::unique_ptr<android_app> app_;
        std::unique_ptr<ndk_helper::assetmgr::AssetManager> assetManager_;
        std::unique_ptr<ndk_helper::mdlmgr::ModelManager> modelManager_;

        EGLDisplay display_{EGL_NO_DISPLAY};
        EGLSurface surface_{EGL_NO_SURFACE};
        EGLContext context_{EGL_NO_CONTEXT};
        EGLint width_{0};
        EGLint height_{0};

        std::unique_ptr<ndk_helper::shdr::Shader> shader_;
        std::unique_ptr<ndk_helper::shdr::Shader> outlined_;
        std::unique_ptr<ndk_helper::shdr::Shader> normalVectorShader_;
        std::unique_ptr<ndk_helper::shdr::Shader> screenShader_;

        GLuint framebuffer_{0}, renderbuffer_{0};
        GLuint framebufferTexture_{0};
        GLuint framebufferVAO_{0}, framebufferVBO_{0};

        void prepare_graphics();
        void create_matrix_uniform_buffer();
        void create_shaders();
        void create_framebuffer();
        void create_renderbuffer();
        void render_framebuffer();
        void destroy_framebuffer();
    public:
        Renderer(android_app *app);
        ~Renderer();

        void render();
        void handle_input();
    };
}

#endif //MESHROTATOR_RENDERER_HPP
