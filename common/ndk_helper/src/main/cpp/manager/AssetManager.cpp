#define STB_IMAGE_IMPLEMENTATION
#include "AssetManager.hpp"
#include "../utils/AndroidOut.hpp"
#include <exception>

ndk_helper::assetmgr::AssetManager::AssetManager(AAssetManager* aAssetManager) : aasetManager_{aAssetManager} {}

ndk_helper::assetmgr::AssetManager::~AssetManager() {
    if (aasetManager_) {
        aasetManager_ = nullptr;
    }
}

const GLuint ndk_helper::assetmgr::AssetManager::loadShader(
    const GLenum type,
    const std::string& path
) const {
    try {
        GLuint shader = createShaderByType(type);
        compileShader(shader, readAssetFromPath(path).c_str());
        if (!isShaderCompiled(shader)) {
            handleShaderCompilationError(shader);

            return 0;
        } else {
            return shader;
        }
    } catch(std::exception& e) {
        aout << e.what() << std::endl;

        return 0;
    }
}

const GLuint ndk_helper::assetmgr::AssetManager::createShaderByType(const GLenum type) const {
    const GLuint shader = glCreateShader(type);
    if (!shader) {
        throw std::runtime_error("Failed to create shader: " + std::to_string(type));
    }

    return shader;
}

void ndk_helper::assetmgr::AssetManager::compileShader(
    const GLuint shader,
    const char* source
) const {
    glShaderSource(shader, 1, &source, nullptr);
    glCompileShader(shader);
}

const std::string
ndk_helper::assetmgr::AssetManager::readAssetFromPath(const std::string& path) const {
    auto assetPtr = AAssetManager_open(aasetManager_, path.c_str(), AASSET_MODE_BUFFER);
    if (!assetPtr) {
        throw std::runtime_error("Failed to open assetPtr: " + path);
    }
    std::string buffer;
    buffer.resize(AAsset_getLength(assetPtr));
    AAsset_read(assetPtr, &buffer[0], AAsset_getLength(assetPtr));
    AAsset_close(assetPtr);

    return buffer;
}

bool ndk_helper::assetmgr::AssetManager::isShaderCompiled(const GLuint shader) const {
    GLint status;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &status);

    return status;
}

void ndk_helper::assetmgr::AssetManager::handleShaderCompilationError(const GLuint shader) const {
    GLint length;
    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);
    if (length == 0) {
        auto infoLog = new char[length];
        glGetShaderInfoLog(shader, length, &length, infoLog);
        aout << "Shader compilation failed: " << infoLog << std::endl;
        delete[] infoLog;
    }
    glDeleteShader(shader);
}

const ndk_helper::mesh::Texture ndk_helper::assetmgr::AssetManager::loadTexture(
    const std::string& path,
    const std::string& type
) const {
    try {
        auto buffer = readAssetFromPath(path);
        auto textureID = generateTextureFromMemory(buffer);

        return {textureID, type};
    } catch (std::exception& e) {
        aout << e.what() << std::endl;

        return {0, ""};
    }
}

const GLuint
ndk_helper::assetmgr::AssetManager::generateTextureFromMemory(const std::string &buffer) const {
    stbi_set_flip_vertically_on_load(true);

    int width, height, numberOfChannels;
    auto data =
        stbi_load_from_memory(
            (const stbi_uc *) buffer.c_str(),
            buffer.length(),
            &width,
            &height,
            &numberOfChannels,
            0
        );

    if (!data) {
        throw std::runtime_error("Failed to load image from memory");
    }

    const GLenum format = getImageFormat(numberOfChannels);
    auto textureID = createTexture(format, width, height, data);

    return textureID;
}

const GLenum ndk_helper::assetmgr::AssetManager::getImageFormat(int &numberOfChannels) const {
    GLenum format;

    switch (numberOfChannels) {
        case 1:
            format = GL_RED;
            break;
        case 3:
            format = GL_RGB;
            break;
        case 4:
            format = GL_RGBA;
            break;
    }

    if (format == GL_NONE) {
        throw std::runtime_error("Failed to get image format: " + std::to_string(numberOfChannels));
    }

    return format;
}

const GLuint ndk_helper::assetmgr::AssetManager::createTexture(
    GLenum format,
    int& width,
    int& height,
    stbi_uc* data
) const {
    GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexImage2D(
        GL_TEXTURE_2D,
        0,
        format,
        width,
        height,
        0,
        format,
        GL_UNSIGNED_BYTE,
        data
    );
    setTextureParameters();
    glBindTexture(GL_TEXTURE_2D, 0);

    return textureID;
}

void ndk_helper::assetmgr::AssetManager::setTextureParameters() const {
    glGenerateMipmap(GL_TEXTURE_2D);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

const std::string ndk_helper::assetmgr::AssetManager::loadModel(const std::string& path) const {
    try {
        return readAssetFromPath(path);
    } catch (std::exception& e) {
        aout << e.what() << std::endl;

        return std::string();
    }
}
