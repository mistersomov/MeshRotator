#include "ModelManager.h"

modelmgr::ModelManager::ModelManager(
    android_app *app
) : asset_manager_{new assetmgr::AssetManager(app->activity->assetManager)} {}