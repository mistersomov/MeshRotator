#include "Renderer.hpp"
#include "utils/Utility.hpp"

#include <memory>
#include <glm/gtc/matrix_transform.hpp>
#include <exception>

bool outlineEnabled;
float xGlobal = 0.0f;
float yGlobal = 0.0f;

rndr::Renderer::~Renderer() {
    if (display_ != EGL_NO_DISPLAY) {
        eglMakeCurrent(display_, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
        if (context_ != EGL_NO_CONTEXT) {
            eglDestroyContext(display_, context_);
        }
        if (surface_ != EGL_NO_SURFACE) {
            eglDestroySurface(display_, surface_);
        }
        eglTerminate(display_);
        display_ = EGL_NO_DISPLAY;
    }
}

void rndr::Renderer::prepare_graphics() {
    constexpr float RED = 0.1f;
    constexpr float GREEN = 0.1f;
    constexpr float BLUE = 0.1f;
    constexpr float ALPHA = 1.0f;
    constexpr EGLint attrs[] = {
        EGL_RENDERABLE_TYPE, EGL_OPENGL_ES3_BIT,
        EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
        EGL_BLUE_SIZE, 8,
        EGL_GREEN_SIZE, 8,
        EGL_RED_SIZE, 8,
        EGL_DEPTH_SIZE, 24,
        EGL_NONE
    };

    auto display = eglGetDisplay(EGL_DEFAULT_DISPLAY);
    eglInitialize(display, nullptr, nullptr);

    EGLint numConfigs;
    eglChooseConfig(display, attrs, nullptr, 0, &numConfigs);

    // get the list of configurations
    std::unique_ptr<EGLConfig[]> configs(new EGLConfig[numConfigs]);
    eglChooseConfig(display, attrs, configs.get(), numConfigs, &numConfigs);

    // choose a config
    auto config = *std::find_if(
        configs.get(),
        configs.get() + numConfigs,
        [&display](const EGLConfig &config) {
            EGLint red, green, blue, depth;
            if (eglGetConfigAttrib(display, config, EGL_RED_SIZE, &red)
                && eglGetConfigAttrib(display, config, EGL_GREEN_SIZE, &green)
                && eglGetConfigAttrib(display, config, EGL_BLUE_SIZE, &blue)
                && eglGetConfigAttrib(display, config, EGL_DEPTH_SIZE, &depth)) {

                aout << "Found config with " << red << ", " << green << ", " << blue << ", "
                     << depth << std::endl;
                return red == 8 && green == 8 && blue == 8 && depth == 24;
            }
            return false;
        }
    );

    aout << "Found " << numConfigs << " configs" << std::endl;
    aout << "Choose " << config << std::endl;

    // create the proper window surface
    EGLint format;
    eglGetConfigAttrib(display, config, EGL_NATIVE_VISUAL_ID, &format);
    EGLSurface surface = eglCreateWindowSurface(display, config, app_->window, nullptr);

    // Create a GLES 3 context
    EGLint contextAttribs[] = {
        EGL_CONTEXT_MAJOR_VERSION,
        3, EGL_CONTEXT_MINOR_VERSION,
        2,
        EGL_NONE
    };
    EGLContext context = eglCreateContext(display, config, nullptr, contextAttribs);

    // get some window metrics
    eglMakeCurrent(display, surface, surface, context);
    eglQuerySurface(display, surface, EGL_WIDTH, &width_);
    eglQuerySurface(display, surface, EGL_HEIGHT, &height_);

    display_ = display;
    surface_ = surface;
    context_ = context;

    glViewport(0, 0, width_, height_);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glEnable(GL_STENCIL_TEST);
    glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
    glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
    glClearColor(RED, GREEN, BLUE, ALPHA);

    aout << "GL_VERSION: " << glGetString(GL_VERSION) << std::endl;
    aout << "GL_RENDERER: " << glGetString(GL_RENDERER) << std::endl;
    aout << "GL_VENDOR: " << glGetString(GL_VENDOR) << std::endl;

    shader_ = std::unique_ptr<ndk_helper::shdr::Shader>(
        asset_manager_->load_shader(
            "shader/cube.vert",
            "shader/cube.frag",
            ""
        )
    );
    outlined_ = std::unique_ptr<ndk_helper::shdr::Shader>(
        asset_manager_->load_shader(
            "shader/cube.vert",
            "shader/outlined.frag",
            ""
        )
    );
    normalShader_ = std::unique_ptr<ndk_helper::shdr::Shader>(
        asset_manager_->load_shader(
            "shader/normalVisualization.vert",
            "shader/normalVisualization.frag",
            "shader/normalVisualization.geom"
        )
    );

    time_manager_->reset();
}

void rndr::Renderer::create_model() {
    std::vector<GLfloat> cubeVertices {
        // positions          // normals           // diffuse_color
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f, // 0 bottom back left
        0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  0.0f, // 1 bottom back right
        0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f, // 2 top back right
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  1.0f, // 3 top back left

        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f, // 4 bottom front left
        0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f, // 5 bottom front right
        0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f, // 6 top front right
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  1.0f, // 7 top front left

        -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f, // 8 left side left bottom
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f, // 9 left side right bottom
        -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f, // 10 left side right top
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  1.0f, // 11 left side left top

        0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f, // 12 right side left bottom
        0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f, // 13 right side right bottom
        0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f, // 14 right side right top
        0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f, // 15 right side left top

        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f, //16  bottom back left
        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f, // 17 bottom front left
        0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f, // 18 bottom front right
        0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  1.0f, // 19 bottom back right

        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f, // 20 top front left
        0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f, // 21 top front right
        0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  1.0f, // 22 top back right
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f, // 23 top back left
    };
    std::vector<GLushort> cubeIndices {
        0, 1, 3,
        2, 3, 1,

        4, 5, 7,
        6, 7, 5,

        8, 9, 11,
        10, 11, 9,

        12, 13, 15,
        14, 15, 13,

        16, 17, 18,
        16, 18, 19,

        20, 21, 23,
        22, 23, 21
    };
    std::vector<ndk_helper::mesh::Texture> textures;
    auto diffuseTexture = asset_manager_->load_texture("model/container.png", "diffuse");
    auto specularTexture = asset_manager_->load_texture("model/container_specular.png", "specular");
    textures.push_back(*diffuseTexture.get());
    textures.push_back(*specularTexture.get());

    models_.emplace_back(ndk_helper::mdl::Model{cubeVertices, cubeIndices, textures});

//    try {
//        auto meshes = asset_manager_->load_mesh("model/bow/bow.obj");
//        models_.emplace_back(mdl::Model{meshes});
//    } catch (std::ios_base::failure& e) {
//        aout << e.what() << std::endl;
//    }
//    catch (std::exception_ptr& e) {
//        aout << "Failed to load model" << std::endl;
//    }
//    catch (std::exception& e) {
//        aout << e.what() << std::endl;
//        return;
//    }
}

void rndr::Renderer::create_matrix_uniform_buffer() {
    unsigned int uniformBlockIndex =
            glGetUniformBlockIndex(shader_->get_id(), "Matrices");
    glUniformBlockBinding(shader_->get_id(), uniformBlockIndex, 0);

    size_t bufferSize = 2 * sizeof(glm::mat4);
    glGenBuffers(1, &uboMatrices_);
    glBindBuffer(GL_UNIFORM_BUFFER, uboMatrices_);
    glBufferData(
        GL_UNIFORM_BUFFER,
        bufferSize,
        nullptr,
        GL_STATIC_DRAW
    );
    glBindBufferRange(
        GL_UNIFORM_BUFFER,
        0,
        uboMatrices_,
        0,
        bufferSize
    );

    glm::mat4 projection = utils::get_projection_matrix(
        static_cast<float>(width_),
        static_cast<float>(height_)
    );
    glBufferSubData(
        GL_UNIFORM_BUFFER,
        0,
        sizeof(glm::mat4),
        glm::value_ptr(projection)
    );
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void rndr::Renderer::render() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    time_manager_->update_time();

    // framebuffer
//    GLuint fbo;
//    glGenFramebuffers(1, &fbo);
//    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
//    unsigned int textureColorbuffer;
//    glGenTextures(1, &textureColorbuffer);
//    glBindTexture(GL_TEXTURE_2D, textureColorbuffer);
//    glTexImage2D(
//        GL_TEXTURE_2D,
//        0,
//        GL_RGB,
//        width_,
//        height_,
//        0,
//        GL_RGB,
//        GL_UNSIGNED_BYTE,
//        nullptr
//    );
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//    glFramebufferTexture2D(
//        GL_FRAMEBUFFER,
//        GL_COLOR_ATTACHMENT0,
//        GL_TEXTURE_2D,
//        textureColorbuffer,
//        0
//    );
//    GLuint rbo;
//    glGenRenderbuffers(1, &rbo);
//    glBindRenderbuffer(GL_RENDERBUFFER, rbo);
//    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width_, height_);
//    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);
//    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
//
//    }
//    glBindFramebuffer(GL_FRAMEBUFFER, 0);
//    //
//
//    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

    for (auto i = models_.begin(), end = models_.end(); i != end; ++i) {
        glStencilFunc(GL_ALWAYS, 1, 0xFF);
        glStencilMask(0xFF);
        glm::mat4 modelView =
            glm::translate(glm::mat4(1.0f), glm::vec3{xGlobal, yGlobal, -5.0f})
                * glm::rotate(
                    glm::mat4(1.0f),
                    glm::radians(time_manager_->delta() * (-50.0f)),
                    glm::vec3{0.2f, 0.3f, 0.0f}
                );
        shader_->activate();
        ndk_helper::shdr::set_mat4(shader_.get(), "MODEL_VIEW", modelView);
        ndk_helper::shdr::set_vec3(shader_.get(), "viewPos", glm::vec3{0.0f, 0.0f, 0.0f});

        ndk_helper::shdr::set_float(shader_.get(), "material.shininess", 64.0f);

        ndk_helper::shdr::set_vec3(shader_.get(), "pointLight.position", utils::get_light_dir());
        ndk_helper::shdr::set_vec3(shader_.get(), "pointLight.ambient", glm::vec3{0.2f, 0.2f, 0.2f});
        ndk_helper::shdr::set_vec3(shader_.get(), "pointLight.diffuse", glm::vec3{0.5f, 0.5f, 0.5f});
        ndk_helper::shdr::set_vec3(shader_.get(), "pointLight.specular", glm::vec3{1.0f, 1.0f, 1.0f});
        ndk_helper::shdr::set_float(shader_.get(), "pointLight.constant", 1.0f);
        ndk_helper::shdr::set_float(shader_.get(), "pointLight.linear", 0.09f);
        ndk_helper::shdr::set_float(shader_.get(), "pointLight.quadratic", 0.032f);

        ndk_helper::shdr::set_vec3(shader_.get(), "dirLight.direction", glm::vec3{-0.2f, -1.0f, -0.3f});
        ndk_helper::shdr::set_vec3(shader_.get(), "dirLight.ambient", glm::vec3{0.05f, 0.05f, 0.05f});
        ndk_helper::shdr::set_vec3(shader_.get(), "dirLight.diffuse", glm::vec3{0.4f, 0.4f, 0.4f});
        ndk_helper::shdr::set_vec3(shader_.get(), "dirLight.specular", glm::vec3{0.5f, 0.5f, 0.5f});i->draw(*shader_);

        normalShader_->activate();
        ndk_helper::shdr::set_mat4(normalShader_.get(), "MODEL_VIEW", modelView);
        i->draw(*normalShader_);

        if (outlineEnabled) {
            float scaled = 1.05f;
            glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
            glStencilMask(0x00);
            glDisable(GL_DEPTH_TEST);
            outlined_->activate();
            modelView =
                    glm::translate(glm::mat4(1.0f), glm::vec3{xGlobal, yGlobal, -5.0f})
                    * glm::rotate(
                            glm::mat4(1.0f),
                            glm::radians(time_manager_->delta() * (-50.0f)),
                            glm::vec3{0.2f, 0.3f, 0.0f}
                    )
                    * glm::scale(glm::mat4(1.0f), glm::vec3{scaled, scaled, scaled});
            ndk_helper::shdr::set_mat4(outlined_.get(), "MODEL_VIEW", modelView);
            ndk_helper::shdr::set_vec3(outlined_.get(), "viewPos", glm::vec3{0.0f, 0.0f, 0.0f});
            ndk_helper::shdr::set_float(outlined_.get(), "uTime", time_manager_->delta());
            i->draw(*outlined_);
            glEnable(GL_DEPTH_TEST);
            glStencilMask(0xFF);
            glStencilFunc(GL_ALWAYS, 0, 0xFF);
        }
    }
//
//
//
//
//    std::vector<GLfloat> quadVertices {
//            -1.0f,  1.0f,  0.0f, 1.0f,
//            -1.0f, -1.0f,  0.0f, 0.0f,
//            1.0f, -1.0f,  1.0f, 0.0f,
//
//            -1.0f,  1.0f,  0.0f, 1.0f,
//            1.0f, -1.0f,  1.0f, 0.0f,
//            1.0f,  1.0f,  1.0f, 1.0f
//    };
//    unsigned int quadVAO, quadVBO;
//    glGenVertexArrays(1, &quadVAO);
//    glGenBuffers(1, &quadVBO);
//    glBindVertexArray(quadVAO);
//    glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
//    glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), quadVertices.data(), GL_STATIC_DRAW);
//    glEnableVertexAttribArray(0);
//    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
//    glEnableVertexAttribArray(1);
//    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
//
//
//    glBindFramebuffer(GL_FRAMEBUFFER, 0);
//    glDisable(GL_DEPTH_TEST);
//    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
//    glClear(GL_COLOR_BUFFER_BIT);
//    screenShader_->activate();
//    ndk_helper::shdr::set_int(screenShader_.get(), "screenTexture", 0);
//    glBindVertexArray(quadVAO);
//    glBindTexture(GL_TEXTURE_2D, textureColorbuffer);	// use the color attachment texture as the texture of the quad plane
//    glDrawArrays(GL_TRIANGLES, 0, 6);

    eglSwapBuffers(display_, surface_);
}

void rndr::Renderer::handle_input() {
    // handle all queued inputs
    auto *inputBuffer = android_app_swap_input_buffers(app_.get());
    if (!inputBuffer) {
        // no inputs yet.
        return;
    }

    // handle motion events (motionEventsCounts can be 0).
    for (auto i = 0; i != inputBuffer->motionEventsCount; ++i) {
        auto &motionEvent = inputBuffer->motionEvents[i];
        auto action = motionEvent.action;

        // Find the pointer index, mask and bitshift to turn it into a readable value.
        auto pointerIndex = (action & AMOTION_EVENT_ACTION_POINTER_INDEX_MASK)
                >> AMOTION_EVENT_ACTION_POINTER_INDEX_SHIFT;
        aout << "Pointer(s): ";

        // get the x and y position of this event if it is not ACTION_MOVE.
        auto &pointer = motionEvent.pointers[pointerIndex];
        auto x = GameActivityPointerAxes_getX(&pointer) * 0.001f;
        auto y = GameActivityPointerAxes_getY(&pointer) * 0.001f;

        // determine the action type and process the event accordingly.
        switch (action & AMOTION_EVENT_ACTION_MASK) {
            case AMOTION_EVENT_ACTION_DOWN:
            case AMOTION_EVENT_ACTION_POINTER_DOWN:
                outlineEnabled = !outlineEnabled;
                aout << "(" << pointer.id << ", " << x << ", " << y << ") "
                     << "Pointer Down";
                break;

            case AMOTION_EVENT_ACTION_CANCEL:
            case AMOTION_EVENT_ACTION_UP:
            case AMOTION_EVENT_ACTION_POINTER_UP:
                aout << "(" << pointer.id << ", " << x << ", " << y << ") "
                     << "Pointer Up";
                break;

            case AMOTION_EVENT_ACTION_MOVE:
                for (auto index = 0; index < motionEvent.pointerCount; index++) {
                    pointer = motionEvent.pointers[index];
                    x = GameActivityPointerAxes_getX(&pointer);
                    y = GameActivityPointerAxes_getY(&pointer);
                    aout << "(" << pointer.id << ", " << x << ", " << y << ")";

                    if (index != (motionEvent.pointerCount - 1)) aout << ",";
                    aout << " ";
                }
                aout << "Pointer Move";
                break;
            default:
                aout << "Unknown MotionEvent Action: " << action;
        }
        aout << std::endl;
    }
    android_app_clear_motion_events(inputBuffer);

    for (auto i = 0; i < inputBuffer->keyEventsCount; i++) {
        auto &keyEvent = inputBuffer->keyEvents[i];
        aout << "Key: " << keyEvent.keyCode << " ";
        switch (keyEvent.action) {
            case AKEY_EVENT_ACTION_DOWN:
                aout << "Key Down";
                break;
            case AKEY_EVENT_ACTION_UP:
                aout << "Key Up";
                break;
            case AKEY_EVENT_ACTION_MULTIPLE:
                aout << "Multiple Key Actions";
                break;
            default:
                aout << "Unknown KeyEvent Action: " << keyEvent.action;
        }
        aout << std::endl;
    }
    // clear the key input count too.
    android_app_clear_key_events(inputBuffer);
}