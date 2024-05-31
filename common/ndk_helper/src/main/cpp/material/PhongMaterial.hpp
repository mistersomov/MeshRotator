#ifndef MESH_ROTATOR_PHONGMATERIAL_HPP
#define MESH_ROTATOR_PHONGMATERIAL_HPP

#include "../core/Material.hpp"
#include "../core/Texture.hpp"

namespace ndk_helper {
    namespace material {
        using Texture = core::Texture;

        struct PhongMaterial : core::Material {
            ~PhongMaterial() override = default;

            Texture ambient;
            Texture diffuse;
            Texture specular;
            Texture normal;
        };
    }
}

#endif //MESH_ROTATOR_PHONGMATERIAL_HPP
