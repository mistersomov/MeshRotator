#ifndef MESH_ROTATOR_PHONGMATERIAL_HPP
#define MESH_ROTATOR_PHONGMATERIAL_HPP

#include "../core/Texture.hpp"

namespace ndk_helper {
    namespace material {
        using Texture = core::Texture;

        struct PhongMaterial {
            Texture ambient;
            Texture diffuse;
            Texture specular;
            Texture normal;
        };
    }
}

#endif //MESH_ROTATOR_PHONGMATERIAL_HPP
