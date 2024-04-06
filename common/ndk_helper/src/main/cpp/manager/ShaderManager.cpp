#include "ShaderManager.hpp"
#include "AssetManager.hpp"
#include "../utils/Utility.hpp"

ndk_helper::shdrmgr::ShaderManager::ShaderManager(AssetManager& assetManager) : assetManager_{assetManager} {}

ndk_helper::shdrmgr::ShaderManager&
ndk_helper::shdrmgr::ShaderManager::instance(AssetManager &assetManager) {
    static ShaderManager instance{assetManager};
    return instance;
}

std::unique_ptr<ndk_helper::shdr::Shader> ndk_helper::shdrmgr::ShaderManager::getShader(
    const std::string &vSource,
    const std::string &fSource,
    const std::string &gSource
) {
    auto vShader = assetManager_.loadShader(
        GL_VERTEX_SHADER,
        vSource
    );
    auto fShader = assetManager_.loadShader(
        GL_FRAGMENT_SHADER,
        fSource
    );
    GLuint gShader;

    if (!gSource.empty()) {
        gShader = assetManager_.loadShader(
            GL_GEOMETRY_SHADER,
            gSource
        );
    }
    auto programId = glCreateProgram();

    glAttachShader(programId, vShader);
    glAttachShader(programId, fShader);
    if (!gSource.empty()) {
        glAttachShader(programId, gShader);
    }
    glLinkProgram(programId);
    glValidateProgram(programId);

    glDeleteShader(vShader);
    glDeleteShader(fShader);
    if (!gSource.empty()) {
        glDeleteShader(gShader);
    }

    return ndk_helper::utils::make_unique<Shader>(programId);
}
