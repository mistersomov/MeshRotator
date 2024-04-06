#ifndef MESH_ROTATOR_AIRCRAFT_HPP
#define MESH_ROTATOR_AIRCRAFT_HPP

#include "NDKHelper.hpp"

namespace model {
    using AssetManager = ndk_helper::assetmgr::AssetManager;
    using TimeManager = ndk_helper::timemgr::TimeManager;
    using SkinnedBody = ndk_helper::core::SkinnedBody;
    using Shader = ndk_helper::shdr::Shader;

    static const glm::vec3 WORLD_Z = glm::vec3(0.0f, 0.0f, 1.0f);

    class Aircraft : public SkinnedBody {
    public:
        Aircraft(AAssetManager* aAssetManager);
        ~Aircraft() override = default;

    private:
        //TimeManager* timeManager_;

        //SkinnedBody propeller_;
        //SkinnedBody fuselage_;
        //SkinnedBody aileronLeft_;
        //SkinnedBody aileronRight_;
        //SkinnedBody elevator_;

        glm::quat fuselageQuat;
        glm::quat aileronLeftQuat;
        glm::quat aileronRightQuat;
        glm::quat elevatorQuat;

        void initTextures();
        void initPropeller();
        void initFuselage();
        void initAilerons();
        void initElevator();
    };
}

#endif //MESH_ROTATOR_AIRCRAFT_HPP
