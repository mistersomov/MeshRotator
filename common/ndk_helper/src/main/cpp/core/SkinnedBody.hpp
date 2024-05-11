#ifndef MESH_ROTATOR_SKINNEDBODY_HPP
#define MESH_ROTATOR_SKINNEDBODY_HPP

#include "CoreBody.hpp"
#include "Bone.hpp"

namespace ndk_helper {
    namespace core {
        constexpr uint32_t BONES_PER_VERTEX = 4;

        struct VertexBoneData {
            VertexBoneData() : ids_(BONES_PER_VERTEX), weights_(BONES_PER_VERTEX) {}

            void fill(uint32_t id, float weight) {
                for (uint32_t i = 0; i != BONES_PER_VERTEX; ++i) {
                    if (weights_[i] == 0.0f) {
                        ids_[i] = id;
                        weights_[i] = weight;

                        return;
                    }
                }
            }

            std::vector<uint32_t> ids_;
            std::vector<float> weights_;
        };

        struct BoneMatrix {
            BoneMatrix(glm::mat4 offset) : offset_{offset}, final_{0.0f} {}

            glm::mat4 offset_;
            glm::mat4 final_;
        };

        class SkinnedBody : public CoreBody {
        public:
            SkinnedBody(const std::string& path, AAssetManager* pAssetManager);
            ~SkinnedBody() override;

            void setBoneTransforms(float timeInSeconds, std::vector<glm::mat4>& transforms);

        protected:
            glm::mat4 rootTransform_;
            std::vector<VertexBoneData> bones_;
            std::map<std::string, uint32_t> boneNameToId_;
            std::vector<BoneMatrix> boneMatrices_;

        private:
            void initBones();
            void initSingleBone(const uint32_t meshIndex, const aiBone* pBone);
            uint32_t getBoneId(const std::string& boneName);
            void fillBonesBuffer();
            void readNodeHierarchy(
                float animationTicks,
                const aiNode* pNode,
                const glm::mat4& parentTransform
            );
            const aiNodeAnim* findNodeAnim(const aiAnimation* pAnimation, const std::string& nodeName);
            void calcInterpolatedScaling(
                aiVector3D& out,
                float animationTimeTicks,
                const aiNodeAnim* pNodeAnim
            );
            uint32_t findScaling(float animationTimeTicks, const aiNodeAnim* pNodeAnim);
            void calcInterpolatedRotation(
                aiQuaternion& out,
                float animationTimeTicks,
                const aiNodeAnim* pNodeAnim
            );
            uint32_t findRotation(float animationTimeTicks, const aiNodeAnim* pNodeAnim);
            void calcInterpolatedPosition(
                aiVector3D& out,
                float animationTimeTicks,
                const aiNodeAnim* pNodeAnim
            );
            uint32_t findPosition(float animationTimeTicks, const aiNodeAnim* pNodeAnim);

            template<typename T>
            uint32_t getKeyIndex(const std::vector<T>& keys, float animationTime) {
                for (uint32_t index = 0; index != keys.size() - 1; ++index) {
                    if (animationTime < keys[index + 1].timeStamp) {
                        return index;
                    }
                }
                return 0;
            };
        };
    }
}

#endif //MESH_ROTATOR_SKINNEDBODY_HPP
