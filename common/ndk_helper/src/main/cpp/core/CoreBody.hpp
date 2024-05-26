#ifndef MESH_ROTATOR_COREBODY_HPP
#define MESH_ROTATOR_COREBODY_HPP

#include "Texture.hpp"
#include "../manager/AssetManager.hpp"
#include "Transformation.hpp"
#include <GLES3/gl32.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <string>
#include <map>

namespace ndk_helper {
    namespace core {
        constexpr uint32_t POSITION = 0;
        constexpr uint32_t NORMAL = 1;
        constexpr uint32_t TEX_COORD = 2;
        constexpr uint32_t TANGENT = 3;
        constexpr uint32_t INDEX = 7;
        constexpr uint32_t BUFFERS_COUNT = 8;

        struct Mesh {
            uint32_t numIndices{0};
            uint32_t baseVertex{0};
            uint32_t baseIndex{0};
        };

        /**
         * Base class for all models. Models are divided into meshes. Used SOA technique.
         */
        class CoreBody {
        public:
            CoreBody(const std::string& path, AAssetManager* pAssetManager);
            virtual ~CoreBody();

        protected:
            virtual void addTextures(std::map<TextureType, std::string> textures) final;
            virtual void onDraw() const final;

            glm::mat4 identityMat_ = glm::mat4(1.0f);
            Transformation transformation_;
            std::vector<Texture> textures_;
            std::vector<GLuint> buffers_;
            std::vector<Mesh> meshes_;
            const aiScene* pScene_ = nullptr;
            uint32_t numVertices_{0};
            uint32_t numIndices_{0};
            GLuint vao_ = -1;

        private:
            void loadModel(const std::string& path);
            void initGeometry();
            void countVerticesAndIndices();
            void reserveSpace();

            void initMeshes();
            void initSingleMesh(const aiMesh* pMesh);

            void fillBuffers();
            void fillPositionBuffer();
            void fillNormalBuffer();
            void fillTexCoordBuffer();
            void fillTangentBuffer();
            void fillIndexBuffer();

            std::vector<glm::vec3> positions_;
            std::vector<glm::vec3> normals_;
            std::vector<glm::vec2> texCoords_;
            std::vector<glm::vec3> tangents_;
            std::vector<GLushort> indices_;

            AAssetManager* assetManager_;
            Assimp::Importer importer;
        };
    }
}

#endif //MESH_ROTATOR_COREBODY_HPP
