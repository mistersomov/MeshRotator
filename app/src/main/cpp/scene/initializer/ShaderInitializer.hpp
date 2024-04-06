#ifndef MESH_ROTATOR_SHADERINITIALIZER_HPP
#define MESH_ROTATOR_SHADERINITIALIZER_HPP

namespace scene {
    class ShaderInitializer {
    public:
        virtual ~ShaderInitializer() = default;

    protected:
        virtual void initShaders() = 0;
        virtual void initUniformBuffers() = 0;
    };
}

#endif //MESH_ROTATOR_SHADERINITIALIZER_HPP
