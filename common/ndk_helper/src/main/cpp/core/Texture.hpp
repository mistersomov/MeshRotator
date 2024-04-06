#ifndef MESH_ROTATOR_TEXTURE_HPP
#define MESH_ROTATOR_TEXTURE_HPP

#include <stdint.h>

namespace ndk_helper {
    namespace core {
        enum class TextureType {
            NONE,
            DIFFUSE,
            NORMAL,
            SPECULAR
        };

        struct Texture {
            TextureType type;
            uint32_t id;
        };
    }
}

#endif //MESH_ROTATOR_TEXTURE_HPP
