#ifndef MESHROTATOR_SCENE_HPP
#define MESHROTATOR_SCENE_HPP

#include "NDKHelper.hpp"

namespace scene {
    /**
     *A scene is an object that knows how to render itself to
     * the screen and knows how to react to input. At any moment in the game,
     * exactly one scene is active, and that scene is the one who decides what gets
     * drawn to the screen and how input is handled.
     */
    class Scene {
    public:
        Scene(float width, float height);
        virtual ~Scene();

        virtual void onCreate();
        virtual void doFrame() = 0;

    protected:
        GLuint framebuffer_{0}, renderbuffer_{0};
        GLuint framebufferTexture_{0};
        GLuint framebufferVAO_{0}, framebufferVBO_{0};

        float width_;
        float height_;

    private:
        static const std::vector<GLfloat> screenVertices;


        //TODO Think about SOLID here
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
