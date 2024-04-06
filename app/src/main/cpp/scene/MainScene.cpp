#include "MainScene.hpp"
#include "../model/aircraft/Aircraft.hpp"
#include <map>
#include <algorithm>
#include <jni.h>

#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/norm.hpp>
#include <glm/gtx/euler_angles.hpp>

// Position of the camera
constexpr float VIEW_POS_X = 0.0f;
constexpr float VIEW_POS_Y = 2.0f;
constexpr float VIEW_POS_Z = 12.0f;

constexpr float TO_RADIANS = M_PI / 180.0f;

// Angle constants
constexpr float MAX_PITCH = 25.0f * TO_RADIANS;
constexpr float MAX_ROLL = 50.0f * TO_RADIANS;
float pos_x = 0;
//constexpr float MAX_AILERON_PITCH = 35.0f * TO_RADIANS;
//constexpr float MAX_ELEVATOR_PITCH = 45.0f * TO_RADIANS;

// Propeller constants
//constexpr float PROP_ROT_SPEED = 1000.0f;

glm::quat fuselageQuat;

scene::MainScene::MainScene(
    float width,
    float height,
    AAssetManager* aAssetManager,
    TimeManager& timeManager
) : Scene(width, height),
    aAssetManager_{aAssetManager},
    timeManager_{std::move(timeManager)},
    viewPos_{VIEW_POS_X, VIEW_POS_Y, VIEW_POS_Z}
{
    std::vector<std::string> skyboxFaces = {
        "texture/cubemap/px.png",
        "texture/cubemap/nx.png",
        "texture/cubemap/py.png",
        "texture/cubemap/ny.png",
        "texture/cubemap/pz.png",
        "texture/cubemap/nz.png",
    };
    skybox_ = ndk_helper::utils::make_unique<Skybox>(skyboxFaces);
}

scene::MainScene::~MainScene() {
    destroyUniformBuffers();
}

void scene::MainScene::onCreate() {
    initShaders();
    initUniformBuffers();
    initModels();
    initLights();
    skybox_->onCreate(aAssetManager_);
}

void scene::MainScene::doFrame() {
    timeManager_.update_time();
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer_);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    glStencilFunc(GL_ALWAYS, 1, 0xFF);
    glStencilMask(0xFF);
    aircraftShader_->activate();
    aircraftShader_->set_vec3("viewPos", viewPos_);
    aircraftShader_->set_vec3("lightPos", ndk_helper::utils::get_light_dir());
    aircraftShader_->set_float("material.shininess", 64.0f);
    glm::mat4 modelMat =
            glm::translate(
                aircraft_->identityMat_,
                aircraft_->transformation_.position_
            )
            * glm::mat4_cast(fuselageQuat)
            * glm::scale(aircraft_->identityMat_, aircraft_->transformation_.scale_);
    aircraftShader_->set_mat4("MODEL", modelMat);
    std::vector<glm::mat4> transforms;
    aircraft_->getBoneTransforms(timeManager_.delta(), transforms);
    for (uint32_t i = 0; i != transforms.size(); ++i) {
        aircraftShader_->set_mat4("BONES[" + std::to_string(i) + "]", transforms[i]);
    }
    aircraft_->onDraw();
    skybox_->onDraw(skyboxShader_.get());
    onDrawFramebuffer();
}

void scene::MainScene::onDrawFramebuffer() const {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glDisable(GL_DEPTH_TEST);
    glClear(GL_COLOR_BUFFER_BIT);
    screenShader_->activate();
    glBindVertexArray(framebufferVAO_);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, framebufferTexture_);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glEnable(GL_DEPTH_TEST);
}

void scene::MainScene::initShaders() {
    ShaderManager& shaderManager = ShaderManager::instance(AssetManager::instance(aAssetManager_));
    aircraftShader_ = shaderManager.getShader(
        "shader/skinning.vert",
        "shader/baseLighting.frag"
    );
    outlineShader_ =  shaderManager.getShader(
        "shader/baseLighting.vert",
        "shader/outlined.frag"
    );
    screenShader_ = shaderManager.getShader(
        "shader/framebufferScreen.vert",
        "shader/framebufferScreen.frag"
    );
    skyboxShader_ = shaderManager.getShader(
        "shader/skybox.vert",
        "shader/skybox.frag"
    );
    skyboxShader_->activate();
    skyboxShader_->set_int("cubemap", 0);
    skyboxShader_->set_mat4("VIEW", glm::mat4(glm::mat3(ndk_helper::utils::get_view_matrix(viewPos_))));
    skyboxShader_->set_mat4("PROJECTION", ndk_helper::utils::get_projection_matrix(width_, height_));
    glUseProgram(0);
}

void scene::MainScene::initUniformBuffers() {
    unsigned int uniformBlockIndexPillar =
            glGetUniformBlockIndex(aircraftShader_->id_, "Matrices");
    glUniformBlockBinding(aircraftShader_->id_, uniformBlockIndexPillar, 0);

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

    auto viewMat = ndk_helper::utils::get_view_matrix(viewPos_);
    auto projMat = ndk_helper::utils::get_projection_matrix(
        width_,
        height_
    );
    glBufferSubData(
        GL_UNIFORM_BUFFER,
        0,
        sizeof(glm::mat4),
        glm::value_ptr(viewMat)
    );
    glBufferSubData(
        GL_UNIFORM_BUFFER,
        sizeof(glm::mat4),
        sizeof(glm::mat4),
        glm::value_ptr(projMat)
    );
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void scene::MainScene::initModels() {
    addAircraft();
}

void scene::MainScene::addAircraft() {
    aircraft_ = std::unique_ptr<model::Aircraft>(new model::Aircraft(aAssetManager_));
    aircraftShader_->activate();
    for (uint32_t i = 0; i != aircraft_->textures_.size(); ++i) {
        glActiveTexture(GL_TEXTURE0 + i);
        std::string name;
        if (aircraft_->textures_[i].type == ndk_helper::core::TextureType::DIFFUSE) {
            name = "diffuse";
        } else if (aircraft_->textures_[i].type == ndk_helper::core::TextureType::SPECULAR) {
            name = "specular";
        } else {
            name = "normal";
        }
        aircraftShader_->set_int("material." + name, i);
        glBindTexture(GL_TEXTURE_2D, aircraft_->textures_[i].id);
    }
    obstacle_ = std::unique_ptr<CoreModel>(new CoreModel("model/aircraft/torus.obj", aAssetManager_));
    obstacle_->transformation_.position_.z = -90.0f;
}

void scene::MainScene::destroyUniformBuffers() {
    glDeleteBuffers(1, &uboMatrices_);
}

void scene::MainScene::initLights() {
    addDirectionalLights();
    //addPointLights();
}

void scene::MainScene::addPointLights() const {
    aircraftShader_->activate();
    aircraftShader_->set_vec3("pointLight.position", ndk_helper::utils::get_light_dir());
    aircraftShader_->set_vec3("pointLight.ambient", glm::vec3{0.2f, 0.2f, 0.2f});
    aircraftShader_->set_vec3("pointLight.diffuse", ndk_helper::utils::get_light_color());
    aircraftShader_->set_vec3("pointLight.specular", glm::vec3{1.0f, 1.0f, 1.0f});
    aircraftShader_->set_float("pointLight.constant", 1.0f);
    aircraftShader_->set_float("pointLight.linear", 0.09f);
    aircraftShader_->set_float("pointLight.quadratic", 0.032f);
}

void scene::MainScene::addDirectionalLights() const {
    aircraftShader_->activate();
    aircraftShader_->set_vec3("dirLight.direction", glm::vec3{-0.2f, -1.0f, -0.3f});
    aircraftShader_->set_vec3("dirLight.ambient", glm::vec3{0.05f, 0.05f, 0.05f});
    aircraftShader_->set_vec3("dirLight.diffuse", glm::vec3{0.4f, 0.4f, 0.4f});
    aircraftShader_->set_vec3("dirLight.specular", glm::vec3{0.5f, 0.5f, 0.5f});
}

void scene::MainScene::renderOutline(CoreModel& model) const {
//    float scaled = 1.05f;
//    glm::mat4 modelMat =
//        glm::translate(identityMat_, model.getPosition())
//            * glm::rotate(
//                identityMat_,
//                glm::radians(timeManager_.delta()*(-50.0f)),
//                glm::vec3{0.0f, 1.0f, 0.0f}
//            )
//            * glm::scale(identityMat_, scaled*(model.getScale()));
//
//    glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
//    glStencilMask(0x00);
//    glDisable(GL_DEPTH_TEST);
//    outlineShader_->activate();
//    outlineShader_->set_mat4("MODEL", modelMat);
//    outlineShader_->set_vec3("viewPos", glm::vec3{0.0f, 0.0f, 0.0f});
//    model.draw(*(outlineShader_.get()));
//    glEnable(GL_DEPTH_TEST);
//    glStencilMask(0xFF);
//    glStencilFunc(GL_ALWAYS, 0, 0xFF);
}

extern "C"
JNIEXPORT void JNICALL
Java_com_mistersomov_meshrotator_MainActivity_updateAircraftAngle(JNIEnv *env, jobject thiz,
                                                                  jfloat x, jfloat y) {
    float pitch = M_PI_2 * y;
    float roll = -M_PI_2 * x;
    //float aileronLeftPitch = M_PI_2 * x;
    if (x == 0.0f && y == 0.0f) {
        pitch = 0.0f;
        roll = 0.0f;
        //aileronLeftPitch = 0.0f;
    }
    float clampedPitch = glm::clamp(pitch, -MAX_PITCH, MAX_PITCH);
    float clampedRoll = glm::clamp(roll, -MAX_ROLL, MAX_ROLL);
//    float clampedAileronLeftPitch = glm::clamp(aileronLeftPitch, -MAX_AILERON_PITCH, MAX_AILERON_PITCH);
//    float clampedElevatorPitch = glm::clamp(pitch, -MAX_ELEVATOR_PITCH, MAX_ELEVATOR_PITCH);

    fuselageQuat = glm::quat_cast(glm::yawPitchRoll(0.0f, clampedPitch, clampedRoll));
    pos_x = x;
}