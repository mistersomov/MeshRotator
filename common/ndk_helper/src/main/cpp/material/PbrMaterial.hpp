#ifndef MESH_ROTATOR_PBRMATERIAL_HPP
#define MESH_ROTATOR_PBRMATERIAL_HPP

#include "../core/Material.hpp"
#include "../core/Texture.hpp"
#include <memory>
#include <string>

namespace ndk_helper {
    namespace material {
        using Texture = core::Texture;

        struct PbrMaterial : public core::Material {
            ~PbrMaterial() override = default;

            Texture albedo;
            Texture normal;
            Texture metallic;
            Texture roughness;
            Texture ao;
            bool isMetal;
        };
    }
}

#endif //MESH_ROTATOR_PBRMATERIAL_HPP
