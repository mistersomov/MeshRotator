#include "Scene.hpp"

const std::vector<GLfloat> scene::Scene::screenVertices = {
    // positions   // texCoords
    -1.0f, 1.0f, 0.0f, 1.0f,
    -1.0f, -1.0f, 0.0f, 0.0f,
    1.0f, -1.0f, 1.0f, 0.0f,

    -1.0f, 1.0f, 0.0f, 1.0f,
    1.0f, -1.0f, 1.0f, 0.0f,
    1.0f, 1.0f, 1.0f, 1.0f
};

scene::Scene::Scene(float width, float height) : width_{width}, height_{height} {
    attachFramebuffer();
}

scene::Scene::~Scene() {
    detachFramebuffer();
}

void scene::Scene::initShaders() {

}

void scene::Scene::initUniformBuffers() {

}

void scene::Scene::initModels() {

}

void scene::Scene::initLights() {

}

void scene::Scene::onCreate() {

}

void scene::Scene::attachFramebuffer() {
    fillFramebufferGLVertexObjects();
    glGenFramebuffers(1, &framebuffer_);
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer_);
    configureFramebufferTexture();
    attachRenderbuffer();
    checkFramebufferStatus();
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void scene::Scene::fillFramebufferGLVertexObjects() {
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
    fillVertexAttribs();
}

void scene::Scene::fillVertexAttribs() {
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(
        0,
        2,
        GL_FLOAT,
        GL_FALSE,
        4 * sizeof(GLfloat),
        nullptr
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
}

void scene::Scene::configureFramebufferTexture() {
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
}

void scene::Scene::attachRenderbuffer() {
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

void scene::Scene::checkFramebufferStatus() const {
    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        aout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
    }
}

void scene::Scene::detachFramebuffer() {
    glDeleteFramebuffers(1, &framebuffer_);
    glDeleteRenderbuffers(1, &renderbuffer_);
    glDeleteTextures(1, &framebufferTexture_);
    glDeleteVertexArrays(1, &framebufferVAO_);
    glDeleteBuffers(1, &framebufferVBO_);
}
