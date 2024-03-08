#include "Renderer.hpp"
#include "utils/Utility.hpp"

#include <memory>
#include <glm/gtc/matrix_transform.hpp>
#include <exception>

bool outlineEnabled;
float xGlobal = 0.0f;
float yGlobal = 0.0f;

rndr::Renderer::Renderer(android_app *app): app_{app},
                                            assetManager_{new ndk_helper::assetmgr::AssetManager(app->activity->assetManager)},
                                            timeManager_{new ndk_helper::timemgr::TimeManager()},
                                            modelManager_{new mdlmgr::ModelManager(*assetManager_)}
{
    prepare_graphics();
    create_shaders();
    modelManager_->loadModelFromPath("model/pillar/pillarsSF.obj");
    create_matrix_uniform_buffer();
    create_framebuffer();
};

rndr::Renderer::~Renderer() {
    destroy_framebuffer();
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

void rndr::Renderer::render() {
    timeManager_->update_time();
    auto models = modelManager_->get_models();

    for (auto i = models.begin(), end = models.end(); i != end; ++i) {
        glBindFramebuffer(GL_FRAMEBUFFER, framebuffer_);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
        glStencilFunc(GL_ALWAYS, 1, 0xFF);
        glStencilMask(0xFF);
        glm::mat4 modelView =
            glm::translate(glm::mat4(1.0f), glm::vec3{xGlobal, yGlobal, -5.0f})
                * glm::rotate(
                    glm::mat4(1.0f),
                    glm::radians(timeManager_->delta() * (-50.0f)),
                    glm::vec3{0.0f, 1.0f, 0.0f}
                )
                * glm::scale(glm::mat4(1.0f), glm::vec3{0.48f, 0.48f, 0.48f});
        shader_->activate();
        shader_->set_mat4("MODEL_VIEW", modelView);
        shader_->set_vec3("viewPos", glm::vec3{0.0f, 0.0f, 0.0f});

        shader_->set_float("material.shininess", 64.0f);

        shader_->set_vec3("pointLight.position", utils::get_light_dir());
        shader_->set_vec3("pointLight.ambient", glm::vec3{0.2f, 0.2f, 0.2f});
        shader_->set_vec3("pointLight.diffuse", utils::get_light_color());
        shader_->set_vec3("pointLight.specular", glm::vec3{1.0f, 1.0f, 1.0f});
        shader_->set_float("pointLight.constant", 1.0f);
        shader_->set_float("pointLight.linear", 0.09f);
        shader_->set_float("pointLight.quadratic", 0.032f);

        shader_->set_vec3("dirLight.direction", glm::vec3{-0.2f, -1.0f, -0.3f});
        shader_->set_vec3("dirLight.ambient", glm::vec3{0.05f, 0.05f, 0.05f});
        shader_->set_vec3("dirLight.diffuse", glm::vec3{0.4f, 0.4f, 0.4f});
        shader_->set_vec3("dirLight.specular", glm::vec3{0.5f, 0.5f, 0.5f});i->draw(*shader_);

        if (outlineEnabled) {
            normalVectorShader_->activate();
            normalVectorShader_->set_mat4("MODEL_VIEW", modelView);
            i->draw(*normalVectorShader_);

            float scaled = 0.5f;
            glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
            glStencilMask(0x00);
            glDisable(GL_DEPTH_TEST);
            outlined_->activate();
            modelView =
                glm::translate(glm::mat4(1.0f), glm::vec3{xGlobal, yGlobal, -5.0f})
                * glm::rotate(
                    glm::mat4(1.0f),
                    glm::radians(timeManager_->delta() * (-50.0f)),
                    glm::vec3{0.0f, 1.0f, 0.0f}
                )
                * glm::scale(glm::mat4(1.0f), glm::vec3{scaled, scaled, scaled});
            outlined_->set_mat4("MODEL_VIEW", modelView);
            outlined_->set_vec3("viewPos", glm::vec3{0.0f, 0.0f, 0.0f});
            outlined_->set_float("uTime", timeManager_->delta());
            i->draw(*outlined_);
            glEnable(GL_DEPTH_TEST);
            glStencilMask(0xFF);
            glStencilFunc(GL_ALWAYS, 0, 0xFF);
        }
    }
    render_framebuffer();
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
    auto config =
        *std::find_if(
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

    timeManager_->reset();
}

void rndr::Renderer::create_matrix_uniform_buffer() {
    GLuint uboMatrices;
    unsigned int uniformBlockIndex =
            glGetUniformBlockIndex(shader_->id_, "Matrices");
    glUniformBlockBinding(shader_->id_, uniformBlockIndex, 0);

    size_t bufferSize = 2 * sizeof(glm::mat4);
    glGenBuffers(1, &uboMatrices);
    glBindBuffer(GL_UNIFORM_BUFFER, uboMatrices);
    glBufferData(
        GL_UNIFORM_BUFFER,
        bufferSize,
        nullptr,
        GL_STATIC_DRAW
    );
    glBindBufferRange(
        GL_UNIFORM_BUFFER,
        0,
        uboMatrices,
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

void rndr::Renderer::create_framebuffer() {
    std::vector<GLfloat> screenVertices {
        // positions   // texCoords
        -1.0f,  1.0f,  0.0f, 1.0f,
        -1.0f, -1.0f,  0.0f, 0.0f,
        1.0f, -1.0f,  1.0f, 0.0f,

        -1.0f,  1.0f,  0.0f, 1.0f,
        1.0f, -1.0f,  1.0f, 0.0f,
        1.0f,  1.0f,  1.0f, 1.0f
    };
    glGenVertexArrays(1, &framebufferVAO_);
    glBindVertexArray(framebufferVAO_);
    glGenBuffers(1, &framebufferVBO_);
    glBindBuffer(GL_ARRAY_BUFFER, framebufferVBO_);
    glBufferData(
        GL_ARRAY_BUFFER,
        sizeof(GLfloat) * screenVertices.size(),
        screenVertices.data(),
        GL_STATIC_DRAW
    );
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(
        0,
        2,
        GL_FLOAT,
        GL_FALSE,
        4 * sizeof(GLfloat),
        (void*)0
    );
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(
        1,
        2,
        GL_FLOAT,
        GL_FALSE,
        4 * sizeof(GLfloat),
        (GLvoid*)(2 * sizeof(GLfloat))
    );
    glBindVertexArray(0);

    glGenFramebuffers(1, &framebuffer_);
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer_);
    glGenTextures(1, &framebufferTexture_);
    glBindTexture(GL_TEXTURE_2D, framebufferTexture_);
    glTexImage2D(
        GL_TEXTURE_2D,
        0,
        GL_RGB,
        width_,
        height_,
        0,
        GL_RGB,
        GL_UNSIGNED_BYTE,
        nullptr
    );
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);
    glFramebufferTexture2D(
        GL_FRAMEBUFFER,
        GL_COLOR_ATTACHMENT0,
        GL_TEXTURE_2D,
        framebufferTexture_,
        0
    );
    create_renderbuffer();
    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        aout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    screenShader_->activate();
    screenShader_->set_int("screenTexture", 0);
}

void rndr::Renderer::create_renderbuffer() {
    glGenRenderbuffers(1, &renderbuffer_);
    glBindRenderbuffer(GL_RENDERBUFFER, renderbuffer_);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width_, height_);
    glFramebufferRenderbuffer(
        GL_FRAMEBUFFER,
        GL_DEPTH_STENCIL_ATTACHMENT,
        GL_RENDERBUFFER,
        renderbuffer_
    );
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
}

void rndr::Renderer::render_framebuffer() {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glDisable(GL_DEPTH_TEST);
    // clear all relevant buffers
    glClear(GL_COLOR_BUFFER_BIT);
    screenShader_->activate();
    glBindVertexArray(framebufferVAO_);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, framebufferTexture_);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glEnable(GL_DEPTH_TEST);
}

void rndr::Renderer::destroy_framebuffer() {
    glDeleteFramebuffers(1, &framebuffer_);
    glDeleteRenderbuffers(1, &renderbuffer_);
    glDeleteTextures(1, &framebufferTexture_);
}

void rndr::Renderer::create_shaders() {
    GLuint programId1 = glCreateProgram();
    glAttachShader(programId1, assetManager_->loadShader(GL_VERTEX_SHADER, "shader/cube.vert"));
    glAttachShader(programId1, assetManager_->loadShader(GL_FRAGMENT_SHADER, "shader/cube.frag"));
    glLinkProgram(programId1);
    shader_ = std::unique_ptr<ndk_helper::shdr::Shader>(new ndk_helper::shdr::Shader(programId1));

    GLuint programId2 = glCreateProgram();
    glAttachShader(programId2, assetManager_->loadShader(GL_VERTEX_SHADER, "shader/cube.vert"));
    glAttachShader(programId2, assetManager_->loadShader(GL_FRAGMENT_SHADER, "shader/outlined.frag"));
    glLinkProgram(programId2);
    outlined_ = std::unique_ptr<ndk_helper::shdr::Shader>(new ndk_helper::shdr::Shader(programId2));

    GLuint programId3 = glCreateProgram();
    glAttachShader(programId3, assetManager_->loadShader(GL_VERTEX_SHADER, "shader/normalVisualization.vert"));
    glAttachShader(programId3, assetManager_->loadShader(GL_GEOMETRY_SHADER, "shader/normalVisualization.geom"));
    glAttachShader(programId3, assetManager_->loadShader(GL_FRAGMENT_SHADER, "shader/normalVisualization.frag"));
    glLinkProgram(programId3);
    normalVectorShader_ = std::unique_ptr<ndk_helper::shdr::Shader>(new ndk_helper::shdr::Shader(programId3));

    GLuint programId4 = glCreateProgram();
    glAttachShader(programId4, assetManager_->loadShader(GL_VERTEX_SHADER, "shader/framebuffer_screen.vert"));
    glAttachShader(programId4, assetManager_->loadShader(GL_FRAGMENT_SHADER, "shader/framebuffer_screen.frag"));
    glLinkProgram(programId4);
    screenShader_ = std::unique_ptr<ndk_helper::shdr::Shader>(new ndk_helper::shdr::Shader(programId4));
}