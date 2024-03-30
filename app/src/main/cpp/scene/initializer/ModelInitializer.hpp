#ifndef MESH_ROTATOR_MODELINITIALIZER_HPP
#define MESH_ROTATOR_MODELINITIALIZER_HPP

namespace scene {
    class ModelInitializer {
    public:
        virtual ~ModelInitializer() = default;

    protected:
        virtual void initModels() = 0;
    };
}

#endif //MESH_ROTATOR_MODELINITIALIZER_HPP
