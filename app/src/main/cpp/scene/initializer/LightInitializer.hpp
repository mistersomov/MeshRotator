#ifndef MESH_ROTATOR_LIGHTINITIALIZER_HPP
#define MESH_ROTATOR_LIGHTINITIALIZER_HPP

namespace scene {
    class LightInitializer {
    public:
        virtual ~LightInitializer() = default;

    protected:
        virtual void initLights() = 0;
    };
}

#endif //MESH_ROTATOR_LIGHTINITIALIZER_HPP
