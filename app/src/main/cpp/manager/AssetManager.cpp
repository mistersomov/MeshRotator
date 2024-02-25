#include "AssetManager.hpp"
#define STB_IMAGE_IMPLEMENTATION
#include "../third_party/stb/stb_image.h"
#include "../utils/AndroidOut.h"

#include <exception>

assetmgr::AssetManager::AssetManager(AAssetManager* assetManager) : assetManager_{assetManager} {}

assetmgr::AssetManager::~AssetManager() {
    if (assetManager_) {
        //delete assetManager_;
    }
}

ndk_helper::shdr::Shader* assetmgr::AssetManager::load_shader(
    const std::string &vPath,
    const std::string &fPath,
    const std::string &gPath
) {
    ndk_helper::shdr::Shader* program = nullptr;
    GLuint vShader = loadShader(GL_VERTEX_SHADER, vPath);
    if (!vShader) {
        return nullptr;
    }
    GLuint fShader = loadShader(GL_FRAGMENT_SHADER, fPath);
    if (!fShader) {
        return nullptr;
    }
    GLuint gShader = 0;
    if (!gPath.empty()) {
        gShader = loadShader(GL_GEOMETRY_SHADER, gPath);
        if (!gShader) {
            return nullptr;
        }
    }
    GLuint programID = glCreateProgram();

    if (programID) {
        glAttachShader(programID, vShader);
        glAttachShader(programID, fShader);
        if (gShader) {
            glAttachShader(programID, gShader);
        }
        glLinkProgram(programID);
        GLint linkStatus = GL_FALSE;
        glGetProgramiv(programID, GL_LINK_STATUS, &linkStatus);
        if (linkStatus != GL_TRUE) {
            GLint logLength = 0;
            glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &logLength);

            // If we fail to link the shdr program, log the result for debugging
            if (logLength) {
                GLchar *log = new GLchar[logLength];
                glGetProgramInfoLog(programID, logLength, nullptr, log);
                aout << "Failed to link program with:\n" << log << std::endl;
                delete[] log;
            }

            glDeleteProgram(programID);
        } else {
            aout << "Program linked successfully" << std::endl;

            program = new ndk_helper::shdr::Shader(programID);
        }
    }

    glDeleteShader(vShader);
    glDeleteShader(fShader);
    if (!gPath.empty() && gShader) {
        glDeleteShader(gShader);
    }

    return program;
}

GLuint assetmgr::AssetManager::loadShader(
    GLenum type,
    const std::string &path
) {
    GLuint shader = glCreateShader(type);

    if (shader) {
        auto asset = AAssetManager_open(assetManager_, path.c_str(), AASSET_MODE_BUFFER);
        if (asset) {
            size_t size = AAsset_getLength(asset);
            std::string buffer;
            buffer.resize(size);
            AAsset_read(asset, &buffer[0], size);
            AAsset_close(asset);
            const char* source = buffer.c_str();
            glShaderSource(shader, 1, &source, nullptr);
            glCompileShader(shader);

            GLint shaderCompiled = 0;
            glGetShaderiv(shader, GL_COMPILE_STATUS, &shaderCompiled);

            // If the shader doesn't compile, log the result to the terminal for debugging
            if (!shaderCompiled) {
                GLint infoLength = 0;
                glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLength);

                if (infoLength) {
                    auto *infoLog = new GLchar[infoLength];
                    glGetShaderInfoLog(shader, infoLength, nullptr, infoLog);
                    aout << "Failed to compile with:\n" << infoLog << std::endl;
                    delete[] infoLog;
                }
                glDeleteShader(shader);
                return 0;
            }
        } else {
            AAsset_close(asset);
        }
    }

    return shader;
}

std::shared_ptr<ndk_helper::mesh::Texture> assetmgr::AssetManager::load_texture(
    const std::string& path,
    const std::string& type
) {
    std::shared_ptr<ndk_helper::mesh::Texture> texture = nullptr;
    auto textureAsset = AAssetManager_open(
        assetManager_,
        path.c_str(),
        AASSET_MODE_BUFFER
    );

    if (textureAsset) {
        size_t length = AAsset_getLength(textureAsset);
        char* buffer = new char[length];
        AAsset_read(textureAsset, buffer, AAsset_getLength(textureAsset));
        AAsset_close(textureAsset);
        int width, height, nrChannels;
        stbi_set_flip_vertically_on_load(true);
        unsigned char* data = stbi_load_from_memory(
            (const stbi_uc*)buffer,
            length,
            &width,
            &height,
            &nrChannels,
            0
        );
        if (data) {
            GLenum format;
            switch (nrChannels) {
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
                aout << "Failed to load texture: " << path << std::endl;
                stbi_image_free(data);
                delete[] buffer;
                return nullptr;
            }

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
            glGenerateMipmap(GL_TEXTURE_2D);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

            texture = std::shared_ptr<ndk_helper::mesh::Texture>(new ndk_helper::mesh::Texture{textureID, type, path});
        }
        stbi_image_free(data);
        delete[] buffer;
    }

    return std::shared_ptr<ndk_helper::mesh::Texture>(texture);
}

const std::string assetmgr::AssetManager::load_model(const std::string& path) const {
    auto asset = AAssetManager_open(assetManager_, path.c_str(), AASSET_MODE_BUFFER);
    if (!asset) {
        aout << "Failed to load asset: " << path << std::endl;
        return nullptr;
    }
    std::string buffer;
    buffer.resize(AAsset_getLength(asset));
    AAsset_read(asset, &buffer[0], AAsset_getLength(asset));
    AAsset_close(asset);

    return buffer;
}