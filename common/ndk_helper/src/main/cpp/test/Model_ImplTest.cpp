#include "../Model_Impl.hpp"
#include "mock/MeshMock.hpp"
#include <gtest/gtest.h>

namespace ndk_helper {
    namespace test {
        using ::testing::AtLeast;

        class Model_ImplTest : public ::testing::Test {
        protected:
            void SetUp() override {
                mesh::Vertex vertex{{0.0f, 1.0f, 2.0f}};
                mesh::Mesh mesh{{vertex}, {}};
                model_ = std::unique_ptr<mdl::Model_Impl>(
                    new mdl::Model_Impl{
                        mesh,
                        glm::vec3(0.0f),
                        glm::vec3(1.0f)
                    }
                );
            }
            void TearDown() override {}

            std::unique_ptr<mdl::Model_Impl> model_;
        };

        TEST_F(Model_ImplTest, AddMesh) {
            // arrange
            mesh::Vertex vertex{{5.0f, 5.0f, 5.0f}};
            mesh::Mesh mesh{{vertex}, {}};

            // act
            model_->addMesh(mesh);

            // assert
            ASSERT_EQ(model_->getMeshes().size(), 2);
            ASSERT_TRUE(model_->getMeshes().back() == mesh);
        }

        TEST_F(Model_ImplTest, GetMeshes) {
            // act && assert
            ASSERT_EQ(model_->getMeshes().size(), 1);
        }

        TEST_F(Model_ImplTest, Draw) {
            // arrange
            shdr::Shader shader{0};
            MeshMock meshMock;

            model_->addMesh(meshMock);
            ON_CALL(meshMock, draw(&shader)).WillByDefault(::testing::Return());

            // act
            model_->draw(shader);

            // assert
            ASSERT_NO_FATAL_FAILURE(model_->draw(shader));
        }

        TEST_F(Model_ImplTest, SetPosition) {
            // arrange
            glm::vec3 newPosition{5.0f, 5.0f, 5.0f};

            // act
            model_->setPosition(newPosition);

            // assert
            ASSERT_EQ(model_->getPosition(), newPosition);
        }

        TEST_F(Model_ImplTest, GetPosition) {
            // act && assert
            ASSERT_EQ(model_->getPosition(), glm::vec3(0.0f));
        }

        TEST_F(Model_ImplTest, SetScale) {
            // arrange
            glm::vec3 newScale{5.0f, 5.0f, 5.0f};

            // act
            model_->setScale(newScale);

            // assert
            ASSERT_EQ(model_->getScale(), newScale);
        }

        TEST_F(Model_ImplTest, GetScale) {
            // act && assert
            ASSERT_EQ(model_->getScale(), glm::vec3(1.0f));
        }
    }
}