#ifndef MESH_ROTATOR_MODELMANAGER_H
#define MESH_ROTATOR_MODELMANAGER_H

#include "AssetManager.hpp"
#include "NDKHelper.hpp"
#include <game-activity/native_app_glue/android_native_app_glue.h>

namespace modelmgr {
    class ModelManager {
        std::unique_ptr<assetmgr::AssetManager> asset_manager_;
        std::vector<ndk_helper::mdl::Model> models_;
    public:
        ModelManager(android_app *app);
    };
}

#endif //MESH_ROTATOR_MODELMANAGER_H