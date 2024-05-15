#ifndef MESHROTATOR_SCENE_HPP
#define MESHROTATOR_SCENE_HPP

#include "NDKHelper.hpp"
#include "initializer/ShaderInitializer.hpp"
#include "initializer/ModelInitializer.hpp"
#include "initializer/LightInitializer.hpp"

namespace scene {
    /**
     *A scene is an object that knows how to render itself to
     * the screen and knows how to react to input. At any moment in the game,
     * exactly one scene is active, and that scene is the one who decides what gets
     * drawn to the screen and how input is handled.
     */
    class Scene : protected ShaderInitializer,
                  protected ModelInitializer,
                  protected LightInitializer {
    public:
        Scene(float width, float height);
        virtual ~Scene();

        virtual void onCreate();
        virtual void doFrame() = 0;

    protected:
        virtual void initShaders() override;
        virtual void initUniformBuffers() override;
        virtual void initModels() override;
        virtual void initLights() override;

        GLuint framebuffer_{0}, renderbuffer_{0};
        GLuint framebufferTexture_{0};
        GLuint framebufferVAO_{0}, framebufferVBO_{0};
        GLuint uboMatrices_;

        float width_;
        float height_;

        glm::mat4 identityMat_ = glm::mat4(1.0f);

    private:
        static const std::vector<GLfloat> screenVertices;

        void attachFramebuffer();
        void fillFramebufferGLVertexObjects();
        void fillVertexAttribs();
        void configureFramebufferTexture();
        void attachRenderbuffer();
        void checkFramebufferStatus() const;
        void detachFramebuffer();
    };
}

#endif //MESHROTATOR_SCENE_HPP
