#include "Aircraft.hpp"
#include <jni.h>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/norm.hpp>
#include <glm/gtx/euler_angles.hpp>

//constexpr float TO_RADIANS = M_PI / 180.0f;

// Angle constants
//constexpr float MAX_PITCH = 25.0f * TO_RADIANS;
//constexpr float MAX_ROLL = 75.0f * TO_RADIANS;
//constexpr float MAX_AILERON_PITCH = 35.0f * TO_RADIANS;
//constexpr float MAX_ELEVATOR_PITCH = 45.0f * TO_RADIANS;

// Propeller constants
//constexpr float PROP_ROT_SPEED = 1000.0f;

model::Aircraft::Aircraft(
    AAssetManager* aAssetManager
) : SkinnedBody("model/aircraft/Capoeira.dae", aAssetManager)
{
    fuselageQuat = glm::angleAxis(0.0f, WORLD_Z);
    aileronLeftQuat = glm::angleAxis(0.0f, WORLD_Z);
    aileronRightQuat = -aileronLeftQuat;
    elevatorQuat = glm::angleAxis(0.0f, WORLD_Z);

    initTextures();
    //initPropeller();
    //initFuselage();
    //initAilerons();
    //initElevator();
}

//void model::Aircraft::onDraw() const {
//
//    SkinnedBody::onDraw();
//
//    modelMat =
//        glm::translate(
//            identityMat_,
//            glm::vec3{
//                aileronLeft_.getPosition().x,
//                aileronLeft_.getPosition().y,
//                aileronLeft_.getPosition().z
//            }
//        ) * glm::mat4_cast(aileronLeftQuat)
//                * glm::scale(identityMat_, getScale());
//    shader.set_mat4("MODEL", modelMat);
//    aileronLeft_.draw(shader);
//
//    modelMat =
//        glm::translate(
//            identityMat_,
//            glm::vec3{
//                aileronRight_.getPosition().x,
//                aileronRight_.getPosition().y,
//                aileronRight_.getPosition().z
//            }
//        ) * glm::mat4_cast(aileronRightQuat)
//                * glm::scale(identityMat_, getScale());
//    shader.set_mat4("MODEL", modelMat);
//    aileronRight_.draw(shader);
//
//    modelMat =
//        glm::translate(
//            identityMat_,
//            glm::vec3{
//                elevator_.getPosition().x,
//                elevator_.getPosition().y,
//                elevator_.getPosition().z
//            }
//        ) * glm::mat4_cast(elevatorQuat)
//                * glm::scale(identityMat_, getScale());
//    shader.set_mat4("MODEL", modelMat);
//    elevator_.draw(shader);
//}

//void model::Aircraft::rotate(int& x, int& y) {
//    float pitch = M_PI_2 * y;
//    float roll = -M_PI_2 * x;
//    float aileronLeftPitch = M_PI_2 * x;
//    if (x == 0.0f && y == 0.0f) {
//        pitch = 0.0f;
//        roll = 0.0f;
//        aileronLeftPitch = 0.0f;
//    }
//    float clampedPitch = glm::clamp(pitch, -MAX_PITCH, MAX_PITCH);
//    float clampedRoll = glm::clamp(roll, -MAX_ROLL, MAX_ROLL);
//    float clampedAileronLeftPitch = glm::clamp(aileronLeftPitch, -MAX_AILERON_PITCH, MAX_AILERON_PITCH);
//    float clampedElevatorPitch = glm::clamp(pitch, -MAX_ELEVATOR_PITCH, MAX_ELEVATOR_PITCH);
//
//    fuselageQuat = glm::quat_cast(glm::yawPitchRoll(0.0f, clampedPitch, clampedRoll));
//    aileronLeftQuat =
//        fuselageQuat
//            * glm::quat_cast(glm::yawPitchRoll(0.0f, clampedAileronLeftPitch, 0.0f));
//    aileronRightQuat =
//        fuselageQuat
//            * glm::quat_cast(glm::yawPitchRoll(0.0f, -clampedAileronLeftPitch, 0.0f));
//    elevatorQuat =
//        fuselageQuat
//            * glm::quat_cast(glm::yawPitchRoll(0.0f, -clampedElevatorPitch, 0.0f));
//}

void model::Aircraft::initTextures() {
    auto textures = std::map<ndk_helper::core::TextureType, std::string>{
        {ndk_helper::core::TextureType::DIFFUSE, "model/aircraft/piper_diffuse.jpg"},
        {ndk_helper::core::TextureType::SPECULAR, "model/aircraft/piper_refl.jpg"}
    };
    addTextures(textures);
}

void model::Aircraft::initPropeller() {
//    ModelManager& modelManager = ModelManager::instance(AssetManager::instance(aAssetManager_));
//    propeller_ = modelManager.getModelFromPath("model/aircraft/propeller.obj");
//    modelManager.applyTextures(propeller_, textures_);
}

void model::Aircraft::initFuselage() {
//    ModelManager& modelManager = ModelManager::instance(AssetManager::instance(aAssetManager_));
//    fuselage_ = modelManager.getModelFromPath("model/aircraft/fuselage.obj");
//    modelManager.applyTextures(fuselage_, textures_);
}

void model::Aircraft::initAilerons() {
//    ModelManager& modelManager = ModelManager::instance(AssetManager::instance(aAssetManager_));
//    aileronLeft_ = modelManager.getModelFromPath("model/aircraft/aileronL.obj");
//    aileronRight_ = modelManager.getModelFromPath("model/aircraft/aileronR.obj");
//    modelManager.applyTextures(aileronLeft_, textures_);
//    modelManager.applyTextures(aileronRight_, textures_);
}

void model::Aircraft::initElevator() {
//    ModelManager& modelManager = ModelManager::instance(AssetManager::instance(aAssetManager_));
//    elevator_ = modelManager.getModelFromPath("model/aircraft/elevator.obj");
//    modelManager.applyTextures(elevator_, textures_);
}
