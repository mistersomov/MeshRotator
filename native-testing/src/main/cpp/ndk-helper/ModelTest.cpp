#include "Model.hpp"
#include "mock/MeshMock.hpp"
#include <gtest/gtest.h>

namespace tests {
    using ::testing::AtLeast;
    using Vertex = ndk_helper::mesh::Vertex;
    using Mesh = ndk_helper::mesh::Mesh;
    using Model = ndk_helper::mdl::Model;
    using Shader = ndk_helper::shdr::Shader;

    class ModelTest : public ::testing::Test {
    protected:
        void SetUp() override {
            Vertex vertex{
                {0.0f, 1.0f, 2.0f},
                {0.0f, 0.0f, 0.0f},
                {0.0f, 0.0f},
                {0.0f, 0.0f, 0.0f},
                {0.0f, 0.0f, 0.0f}
            };
            Mesh mesh{{vertex}, {}};
            model_ = std::unique_ptr<Model>(
                new Model{
                    mesh,
                    glm::vec3(0.0f),
                    glm::vec3(1.0f)
                }
            );
        }
        void TearDown() override {}

        std::unique_ptr<Model> model_;
    };

    TEST_F(ModelTest, AddMesh) {
        // arrange
        Vertex vertex{
            {5.0f, 5.0f, 5.0f},
            {0.0f, 0.0f, 0.0f},
            {0.0f, 0.0f},
            {0.0f, 0.0f, 0.0f},
            {0.0f, 0.0f, 0.0f}
        };
        Mesh mesh{{vertex}, {}};

        // act
        model_->addMesh(mesh);

        // assert
        ASSERT_EQ(model_->getMeshes().size(), 2);
        ASSERT_TRUE(model_->getMeshes().back() == mesh);
    }

    TEST_F(ModelTest, GetMeshes) {
        // act && assert
        ASSERT_EQ(model_->getMeshes().size(), 1);
    }

    TEST_F(ModelTest, Draw) {
        // arrange
        Shader shader{0};
        MeshMock meshMock;

        model_->addMesh(meshMock);
        ON_CALL(meshMock, draw(&shader)).WillByDefault(::testing::Return());

        // act
        model_->draw(shader);

        // assert
        ASSERT_NO_FATAL_FAILURE(model_->draw(shader));
    }

    TEST_F(ModelTest, SetPosition) {
        // arrange
        glm::vec3 newPosition{5.0f, 5.0f, 5.0f};

        // act
        model_->setPosition(newPosition);

        // assert
        ASSERT_EQ(model_->getPosition(), newPosition);
    }

    TEST_F(ModelTest, GetPosition) {
        // act && assert
        ASSERT_EQ(model_->getPosition(), glm::vec3(0.0f));
    }

    TEST_F(ModelTest, SetScale) {
        // arrange
        glm::vec3 newScale{5.0f, 5.0f, 5.0f};

        // act
        model_->setScale(newScale);

        // assert
        ASSERT_EQ(model_->getScale(), newScale);
    }

    TEST_F(ModelTest, GetScale) {
        // act && assert
        ASSERT_EQ(model_->getScale(), glm::vec3(1.0f));
    }
}