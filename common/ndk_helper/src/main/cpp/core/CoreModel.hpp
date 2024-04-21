#ifndef MESH_ROTATOR_COREMODEL_HPP
#define MESH_ROTATOR_COREMODEL_HPP

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
        class CoreModel {
        public:
            CoreModel(const std::string& path, AAssetManager* pAssetManager);
            virtual ~CoreModel();

        protected:
            virtual void onDraw() const;
            virtual void addTextures(std::map<TextureType, std::string> textures) final;

            Transformation transformation_;

        private:
            void loadModel(const std::string& path);
            void initGeometry(const aiScene* pScene);
            void countVerticesAndIndices(const aiScene* pScene, uint32_t& NumVertices, uint32_t& NumIndices);
            void initAllMeshes(const aiScene* pScene);
            void initSingleMesh(const aiMesh* pMesh);
            void fillBuffers();
            void fillPositionBuffer();
            void fillNormalBuffer();
            void fillTexCoordBuffer();
            void fillTangentBuffer();
            void fillBiTangentBuffer();
            void fillIndexBuffer();

            struct Mesh {
                uint32_t numIndices{0};
                uint32_t baseVertex{0};
                uint32_t baseIndex{0};
            };

            std::vector<GLuint> buffers_;
            std::vector<Mesh> meshes_;
            std::vector<Texture> textures_;
            std::vector<glm::vec3> positions_;
            std::vector<glm::vec3> normals_;
            std::vector<glm::vec2> texCoords_;
            std::vector<glm::vec3> tangents_;
            std::vector<glm::vec3> biTangents_;
            std::vector<GLushort> indices_;

            GLuint vao_ = -1;

            AAssetManager* assetManager_;
            Assimp::Importer importer;
        };
    }
}

#endif //MESH_ROTATOR_COREMODEL_HPP
