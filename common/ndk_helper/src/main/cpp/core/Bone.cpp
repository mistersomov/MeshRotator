#include "Bone.hpp"

ndk_helper::core::Bone::Bone(
    uint32_t id,
    const std::string& name,
    const aiNodeAnim* pChannel
) : id_{id}, name_{name}, transformation_{1.0f} {
    numPositions_ = pChannel->mNumPositionKeys;
    numRotations_ = pChannel->mNumRotationKeys;
    numScales_ = pChannel->mNumScalingKeys;

    for (uint32_t i = 0; i != numPositions_; ++i) {
        aiVector3D position = pChannel->mPositionKeys[i].mValue;
        float timeStamp = pChannel->mPositionKeys[i].mTime;
        positions_.push_back({utils::toGlmVec3(position), timeStamp});
    }
    for (uint32_t i = 0; i != numRotations_; ++i) {
        aiQuaternion rotation = pChannel->mRotationKeys[i].mValue;
        float timeStamp = pChannel->mRotationKeys[i].mTime;
        rotations_.push_back({utils::toGlmQuat(rotation), timeStamp});
    }
    for (uint32_t i = 0; i != numScales_; ++i) {
        aiVector3D scale = pChannel->mScalingKeys[i].mValue;
        float timeStamp = pChannel->mScalingKeys[i].mTime;
        scales_.push_back({utils::toGlmVec3(scale), timeStamp});
    }
}

const uint32_t& ndk_helper::core::Bone::getId() const {
    return id_;
}

const std::string& ndk_helper::core::Bone::getName() const {
    return name_;
}

const glm::mat4& ndk_helper::core::Bone::getTransformation() const {
    return transformation_;
}

void ndk_helper::core::Bone::update(float animationTime) {
    transformation_ =
            interpolatePosition(animationTime)
                * interpolateRotation(animationTime)
                * interpolateScaling(animationTime);
}

glm::mat4 ndk_helper::core::Bone::interpolateScaling(float animationTime) {
    if (numScales_ == 1) {
        return glm::scale(glm::mat4(1.0f), scales_[0].value);
    }

    uint32_t index0 = getKeyIndex(scales_, animationTime);
    uint32_t index1 = index0 + 1;
    float scaleFactor = getScaleFactor(
        rotations_[index0].timeStamp,
        rotations_[index1].timeStamp,
        animationTime
    );
    glm::vec3 scale = glm::mix(scales_[index0].value, scales_[index1].value, scaleFactor);

    return glm::scale(glm::mat4(1.0f), scale);
}

float ndk_helper::core::Bone::getScaleFactor(
    float lastTimeStamp,
    float nextTimeStamp,
    float animationTime
) {
    float midway = animationTime - lastTimeStamp;
    float framesDiff = nextTimeStamp - lastTimeStamp;

    return midway / framesDiff;
}

glm::mat4 ndk_helper::core::Bone::interpolateRotation(float animationTime) {
    if (numRotations_ == 1) {
        return glm::mat4_cast(glm::normalize(rotations_[0].value));
    }

    uint32_t index0 = getKeyIndex(rotations_, animationTime);
    uint32_t index1 = index0 + 1;
    float scaleFactor = getScaleFactor(
        rotations_[index0].timeStamp,
        rotations_[index1].timeStamp,
        animationTime
    );
    glm::quat rotation = glm::normalize(
        glm::slerp(rotations_[index0].value, rotations_[index1].value, scaleFactor)
    );

    return glm::mat4_cast(rotation);
}

glm::mat4 ndk_helper::core::Bone::interpolatePosition(float animationTime) {
    if (numPositions_ == 1) {
        return glm::translate(glm::mat4(1.0f), positions_[0].value);
    }

    uint32_t index0 = getKeyIndex(positions_, animationTime);
    uint32_t index1 = index0 + 1;
    float scaleFactor = getScaleFactor(
        positions_[index0].timeStamp,
        positions_[index1].timeStamp,
        animationTime
    );
    glm::vec3 position = glm::mix(
        positions_[index0].value,
        positions_[index1].value,
        scaleFactor
    );

    return glm::translate(glm::mat4(1.0f), position);
}
