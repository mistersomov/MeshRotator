#ifndef MESH_ROTATOR_BONE_HPP
#define MESH_ROTATOR_BONE_HPP

#include "../utils/AssimpHelper.hpp"

namespace ndk_helper {
    namespace core {
        struct KeyPosition {
            glm::vec3 value;
            float timeStamp;
        };

        struct KeyRotation {
            glm::quat value;
            float timeStamp;
        };

        struct KeyScale {
            glm::vec3 value;
            float timeStamp;
        };

        class Bone {
        public:
            Bone(uint32_t id, const std::string& name, const aiNodeAnim* pChannel);

            inline const uint32_t& getId() const;
            inline const std::string& getName() const;
            inline const glm::mat4& getTransformation() const;
            void update(float animationTime);

        private:
            float getScaleFactor(float lastTimeStamp, float nextTimeStamp, float animationTime);
            glm::mat4 interpolateScaling(float animationTime);
            glm::mat4 interpolateRotation(float animationTime);
            glm::mat4 interpolatePosition(float animationTime);

            template<typename T>
            uint32_t getKeyIndex(const std::vector<T>& keys, float animationTime) {
                for (uint32_t index = 0; index != keys.size() - 1; ++index) {
                    if (animationTime < keys[index + 1].timeStamp) {
                        return index;
                    }
                }
                return 0;
            };

            uint32_t id_{0};
            std::string name_;
            uint32_t numPositions_{0};
            uint32_t numRotations_{0};
            uint32_t numScales_{0};
            std::vector<KeyPosition> positions_;
            std::vector<KeyRotation> rotations_;
            std::vector<KeyScale> scales_;
            glm::mat4 transformation_;
        };
    }
}

#endif //MESH_ROTATOR_BONE_HPP
