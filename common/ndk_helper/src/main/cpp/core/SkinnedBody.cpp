#include "SkinnedBody.hpp"
#include "../utils/Utility.hpp"

constexpr uint32_t BONE = 5;
constexpr uint32_t BONE_WEIGHT = 6;

ndk_helper::core::SkinnedBody::SkinnedBody(
    const std::string& path,
    AAssetManager* pAssetManager
) : CoreBody(path, pAssetManager) {
    initBones();
    fillBonesBuffer();
}

ndk_helper::core::SkinnedBody::~SkinnedBody() {
    glDisableVertexAttribArray(BONE);
    glDisableVertexAttribArray(BONE_WEIGHT);
}

void ndk_helper::core::SkinnedBody::initBones() {
    bones_.reserve(numVertices_);
    for (uint32_t i = 0; i != pScene_->mNumMeshes; ++i) {
        const aiMesh* pMesh = pScene_->mMeshes[i];
        for (uint32_t j = 0; j != pMesh->mNumBones; ++j) {
            initSingleBone(i, pMesh->mBones[j]);
        }
    }
}

void ndk_helper::core::SkinnedBody::initSingleBone(const uint32_t meshIndex, const aiBone* pBone) {
    uint32_t id = getBoneId(pBone->mName.C_Str());

    if (id == boneMatrices_.size()) {
        auto offset = ndk_helper::utils::toGlmMat4(pBone->mOffsetMatrix);
        BoneMatrix boneMatrix(offset);
        boneMatrices_.push_back(boneMatrix);
    }
    for (uint32_t i = 0; i != pBone->mNumWeights; ++i) {
        const aiVertexWeight& vw = pBone->mWeights[i];
        uint32_t globalVertexId = meshes_[meshIndex].baseVertex + vw.mVertexId;
        bones_[globalVertexId].fill(id, vw.mWeight);
    }
}

uint32_t ndk_helper::core::SkinnedBody::getBoneId(const std::string& boneName) {
    uint32_t id;

    if (boneNameToId_.find(boneName) == boneNameToId_.end()) {
        id = boneNameToId_.size();
        boneNameToId_[boneName] = id;
    } else {
        id = boneNameToId_[boneName];
    }

    return id;
}

void ndk_helper::core::SkinnedBody::fillBonesBuffer() {
    glBindVertexArray(vao_);
    glBindBuffer(GL_ARRAY_BUFFER, buffers_[BONE]);
    glBufferData(
        GL_ARRAY_BUFFER,
        bones_.size() * sizeof(bones_[0]),
        bones_.data(),
        GL_STATIC_DRAW
    );
    glEnableVertexAttribArray(BONE);
    glVertexAttribIPointer(
        BONE,
        BONES_PER_VERTEX,
        GL_INT,
        sizeof(VertexBoneData),
        nullptr
    );
    glEnableVertexAttribArray(BONE_WEIGHT);
    glVertexAttribPointer(
        BONE_WEIGHT,
        BONES_PER_VERTEX,
        GL_FLOAT,
        GL_FALSE,
        sizeof(VertexBoneData),
        (void *)offsetof(VertexBoneData, weights_)
    );
    glBindVertexArray(0);
}

void ndk_helper::core::SkinnedBody::setBoneTransforms(
    float timeInSeconds,
    std::vector<glm::mat4>& transforms
) {
    float ticksPerSecond = static_cast<float>(pScene_->mAnimations[0]->mTicksPerSecond != 0 ?
            pScene_->mAnimations[0]->mTicksPerSecond : 25.0f);
    float timeInTicks = timeInSeconds * ticksPerSecond;
    float animationTicks = fmod(timeInTicks, static_cast<float>(pScene_->mAnimations[0]->mDuration));

    readNodeHierarchy(animationTicks, pScene_->mRootNode, identityMat_);
    transforms.resize(boneMatrices_.size());

    for (uint32_t i = 0; i != boneMatrices_.size(); ++i) {
        transforms[i] = boneMatrices_[i].final_;
    }
}

void ndk_helper::core::SkinnedBody::readNodeHierarchy(
    float animationTicks,
    const aiNode* pNode,
    const glm::mat4& parentTransform
) {
    std::string nodeName(pNode->mName.data);
    const aiAnimation* pAnimation = pScene_->mAnimations[0];
    auto nodeTransformation = ndk_helper::utils::toGlmMat4(pNode->mTransformation);
    auto pNodeAnim = findNodeAnim(pAnimation, nodeName);

    if (pNodeAnim) {
        aiVector3D scaling;
        calcInterpolatedScaling(scaling, animationTicks, pNodeAnim);
        glm::mat4 scaleMat =
                glm::mat4(
                    glm::vec4(scaling.x, 0.0f, 0.0f, 0.0f),
                    glm::vec4(0.0f, scaling.y, 0.0f, 0.0f),
                    glm::vec4(0.0f, 0.0f, scaling.z, 0.0f),
                    glm::vec4(0.0f, 0.0f, 0.0f, 1.0f)
                );

        aiQuaternion rotation;
        calcInterpolatedRotation(rotation, animationTicks, pNodeAnim);
        glm::mat4 rotationMat =
                glm::mat4(
                    rotation.GetMatrix().a1, rotation.GetMatrix().b1, rotation.GetMatrix().c1, 0.0f,
                    rotation.GetMatrix().a2, rotation.GetMatrix().b2, rotation.GetMatrix().c2, 0.0f,
                    rotation.GetMatrix().a3, rotation.GetMatrix().b3, rotation.GetMatrix().c3, 0.0f,
                    0.0f, 0.0f, 0.0f, 1.0f
                );

        aiVector3D position;
        calcInterpolatedPosition(position, animationTicks, pNodeAnim);
        glm::mat4 positionMat = glm::translate(identityMat_, glm::vec3(position.x, position.y, position.z));

        nodeTransformation = positionMat * rotationMat * scaleMat;
    }

    auto globalTransformation = parentTransform * nodeTransformation;

    if (boneNameToId_.find(nodeName) != boneNameToId_.end()) {
        uint32_t id = boneNameToId_[nodeName];
        boneMatrices_[id].final_ = rootTransform_ * globalTransformation * boneMatrices_[id].offset_;
    }
    for (uint32_t i = 0; i != pNode->mNumChildren; ++i) {
        readNodeHierarchy(animationTicks, pNode->mChildren[i], globalTransformation);
    }
}

const aiNodeAnim *ndk_helper::core::SkinnedBody::findNodeAnim(
    const aiAnimation* pAnimation,
    const std::string& nodeName
) {
    for (uint32_t i = 0; i != pAnimation->mNumChannels; ++i) {
        const aiNodeAnim* pNodeAnim = pAnimation->mChannels[i];
        if (std::string(pNodeAnim->mNodeName.data) == nodeName) {
            return pNodeAnim;
        }
    }
    return nullptr;
}

void ndk_helper::core::SkinnedBody::calcInterpolatedScaling(
    aiVector3D& out,
    float animationTimeTicks,
    const aiNodeAnim* pNodeAnim
) {
    if (pNodeAnim->mNumScalingKeys == 1) {
        out = pNodeAnim->mScalingKeys[0].mValue;
        return;
    }

    uint32_t index0 = findScaling(animationTimeTicks, pNodeAnim);
    uint32_t index1 = index0 + 1;
    if (index1 < pNodeAnim->mNumScalingKeys) {
        float t0 = static_cast<float>(pNodeAnim->mScalingKeys[index0].mTime);
        float t1 = static_cast<float>(pNodeAnim->mScalingKeys[index1].mTime);
        float deltaTime = t1 - t0;
        float factor = (animationTimeTicks - t0) / deltaTime;
        if (factor >= 0.0f &&  factor <= 1.0f) {
            const aiVector3D& start = pNodeAnim->mScalingKeys[index0].mValue;
            const aiVector3D& end = pNodeAnim->mScalingKeys[index1].mValue;
            aiVector3D delta = end - start;
            out = start + factor * delta;
        }
    }
}

uint32_t ndk_helper::core::SkinnedBody::findScaling(
    float animationTimeTicks,
    const aiNodeAnim* pNodeAnim
) {
    for (uint32_t i = 0; i != pNodeAnim->mNumScalingKeys - 1; ++i) {
        float t0 = static_cast<float>(pNodeAnim->mScalingKeys[i + 1].mTime);
        if (animationTimeTicks < t0) {
            return i;
        }
    }
    return 0;
}

void ndk_helper::core::SkinnedBody::calcInterpolatedRotation(
    aiQuaternion& out,
    float animationTimeTicks,
    const aiNodeAnim* pNodeAnim
) {
    if (pNodeAnim->mNumRotationKeys == 1) {
        out = pNodeAnim->mRotationKeys[0].mValue;
        return;
    }

    uint32_t index0 = findRotation(animationTimeTicks, pNodeAnim);
    uint32_t index1 = index0 + 1;
    if (index1 < pNodeAnim->mNumRotationKeys) {
        float t0 = static_cast<float>(pNodeAnim->mRotationKeys[index0].mTime);
        float t1 = static_cast<float>(pNodeAnim->mRotationKeys[index1].mTime);
        float deltaTime = t1 - t0;
        float factor = (animationTimeTicks - t0) / deltaTime;
        if (factor >= 0.0f && factor <= 1.0f) {
            const aiQuaternion& start = pNodeAnim->mRotationKeys[index0].mValue;
            const aiQuaternion& end = pNodeAnim->mRotationKeys[index1].mValue;
            aiQuaternion::Interpolate(out, start, end, factor);
            out.Normalize();
        }
    }
}

uint32_t ndk_helper::core::SkinnedBody::findRotation(
    float animationTimeTicks,
    const aiNodeAnim* pNodeAnim
) {
    for (uint32_t i = 0; i != pNodeAnim->mNumRotationKeys - 1; ++i) {
        float t0 = static_cast<float>(pNodeAnim->mRotationKeys[i + 1].mTime);
        if (animationTimeTicks < t0) {
            return i;
        }
    }
    return 0;
}

void ndk_helper::core::SkinnedBody::calcInterpolatedPosition(
    aiVector3D& out,
    float animationTimeTicks,
    const aiNodeAnim* pNodeAnim
) {
    if (pNodeAnim->mNumPositionKeys == 1) {
        out = pNodeAnim->mPositionKeys[0].mValue;
        return;
    }

    uint32_t index0 = findPosition(animationTimeTicks, pNodeAnim);
    uint32_t index1 = index0 + 1;
    if (index1 < pNodeAnim->mNumPositionKeys) {
        float t0 = static_cast<float>(pNodeAnim->mPositionKeys[index0].mTime);
        float t1 = static_cast<float>(pNodeAnim->mPositionKeys[index1].mTime);
        float deltaTime = t1 - t0;
        float factor = (animationTimeTicks - t0) / deltaTime;
        if (factor >= 0.0f && factor <= 1.0f) {
            const aiVector3D& start = pNodeAnim->mPositionKeys[index0].mValue;
            const aiVector3D& end = pNodeAnim->mPositionKeys[index1].mValue;
            aiVector3D delta = end - start;
            out = start + factor * delta;
        }
    }
}

uint32_t ndk_helper::core::SkinnedBody::findPosition(
    float animationTimeTicks,
    const aiNodeAnim *pNodeAnim
) {
    for (uint32_t i = 0; i != pNodeAnim->mNumPositionKeys - 1; ++i) {
        float t0 = static_cast<float>(pNodeAnim->mPositionKeys[i + 1].mTime);
        if (animationTimeTicks < t0) {
            return i;
        }
    }
    return 0;
}
