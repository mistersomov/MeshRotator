#include "MainScene.hpp"
#include <map>
#include <algorithm>

// Position of the camera
constexpr float VIEW_POS_X = 0.0f;
constexpr float VIEW_POS_Y = 4.0f;
constexpr float VIEW_POS_Z = 10.0f;

//Physics constants
constexpr float TURBULENCE = 5.0f;

scene::MainScene::MainScene(
    float width,
    float height,
    AAssetManager* aAssetManager,
    TimeManager& timeManager
) : Scene(width, height),
    aAssetManager_{aAssetManager},
    timeManager_{std::move(timeManager)}
{
    camera_ = ndk_helper::utils::make_unique<Camera>(
        glm::vec3{VIEW_POS_X, VIEW_POS_Y, VIEW_POS_Z},
        width,
        height
    );
}

scene::MainScene::~MainScene() {
    destroyUniformBuffers();
}

void scene::MainScene::onCreate() {
    initShaders();
    initUniformBuffers();
    initModels();
    initLights();
    initSkybox();
}

void scene::MainScene::doFrame() {
    timeManager_.update();
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer_);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    glStencilFunc(GL_ALWAYS, 1, 0xFF);
    glStencilMask(0xFF);
    std::for_each(
        models_.begin(),
        models_.end(),
        [this](Model& model) {
            renderModels(model);
        }
    );
    skybox_->onDraw();
    renderFramebuffer();
}

void scene::MainScene::renderModels(scene::Model &model) const {
    glm::mat4 modelMat =
        glm::translate(
            identityMat_,
            glm::vec3{
                model.getPosition().x,
                model.getPosition().y,
                sin(timeManager_.getDeltaTotal() * 2.0f * model.getPosition().z)
            }
        )
            * glm::rotate(
                identityMat_,
                glm::radians(sin(timeManager_.getDeltaTotal() * TURBULENCE)),
                glm::vec3{0.0f, 0.0f, 1.0f}
            )
            * glm::scale(identityMat_, model.getScale());

    pillarShader_->activate();
    pillarShader_->set_mat4("MODEL", modelMat);
    pillarShader_->set_vec3("viewPos", camera_->position);
    pillarShader_->set_vec3("lightPos", ndk_helper::utils::get_light_dir());
    pillarShader_->set_float("material.shininess", 64.0f);
    model.draw(*(pillarShader_.get()));
}

void scene::MainScene::renderFramebuffer() const {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glDisable(GL_DEPTH_TEST);
    glClear(GL_COLOR_BUFFER_BIT);
    screenShader_->activate();
    glBindVertexArray(framebufferVAO_);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, framebufferTexture_);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glEnable(GL_DEPTH_TEST);
}

void scene::MainScene::initShaders() {
    ShaderManager& shaderManager = ShaderManager::instance(AssetManager::instance(aAssetManager_));
    pillarShader_ = shaderManager.getShader(
        "shader/baseLighting.vert",
        "shader/baseLighting.frag"
    );
    outlineShader_ =  shaderManager.getShader(
        "shader/baseLighting.vert",
        "shader/outlined.frag"
    );
    screenShader_ = shaderManager.getShader(
        "shader/framebufferScreen.vert",
        "shader/framebufferScreen.frag"
    );
    skyboxShader_ = shaderManager.getShader(
        "shader/skybox.vert",
        "shader/skybox.frag"
    );
    skyboxShader_->activate();
    skyboxShader_->set_int("cubemap", 0);
    skyboxShader_->set_mat4("VIEW", glm::mat4(glm::mat3(camera_->getViewMatrix())));
    skyboxShader_->set_mat4("PROJECTION", camera_->getProjectionMatrix());
    glUseProgram(0);
}

void scene::MainScene::initUniformBuffers() {
    unsigned int uniformBlockIndexPillar =
            glGetUniformBlockIndex(pillarShader_->id_, "Matrices");
    glUniformBlockBinding(pillarShader_->id_, uniformBlockIndexPillar, 0);

    size_t bufferSize = 2 * sizeof(glm::mat4);
    glGenBuffers(1, &uboMatrices_);
    glBindBuffer(GL_UNIFORM_BUFFER, uboMatrices_);
    glBufferData(
        GL_UNIFORM_BUFFER,
        bufferSize,
        nullptr,
        GL_STATIC_DRAW
    );
    glBindBufferRange(
        GL_UNIFORM_BUFFER,
        0,
        uboMatrices_,
        0,
        bufferSize
    );
    glBufferSubData(
        GL_UNIFORM_BUFFER,
        0,
        sizeof(glm::mat4),
        glm::value_ptr(camera_->getViewMatrix())
    );
    glBufferSubData(
        GL_UNIFORM_BUFFER,
        sizeof(glm::mat4),
        sizeof(glm::mat4),
        glm::value_ptr(camera_->getProjectionMatrix())
    );
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void scene::MainScene::initModels() {
    //addPillar();
    //addBrokenPillar();
    addAircraft();
}

void scene::MainScene::addPillar() {
    ModelManager& modelManager = ModelManager::instance(AssetManager::instance(aAssetManager_));
    auto pillar = modelManager.getModelFromPath("model/pillar/pillarsSF.obj");
    auto textures = std::map<ndk_helper::mesh::TextureType, std::string>{
        {ndk_helper::mesh::TextureType::DIFFUSE, "model/pillar/pillar_1_BaseColor.png"},
        {ndk_helper::mesh::TextureType::NORMAL, "model/pillar/pillar_1_NormaL_GL.png"}
    };

    modelManager.applyTextures(pillar, textures);
    pillar.setPosition({-8.0f, 0.0f, -1.0f});
    pillar.setScale(glm::vec3(1.0f));
    models_.emplace_back(pillar);
}

void scene::MainScene::addBrokenPillar() {
    ModelManager& modelManager = ModelManager::instance(AssetManager::instance(aAssetManager_));
    auto pillar = modelManager.getModelFromPath("model/pillar/pillarsSmallSF.obj");
    auto textures = std::map<ndk_helper::mesh::TextureType, std::string>{
        {ndk_helper::mesh::TextureType::DIFFUSE, "model/pillar/BrokenPillar_BaseColor.png"},
        {ndk_helper::mesh::TextureType::NORMAL, "model/pillar/BrokenPillar_NormaL_GL.png"}
    };

    modelManager.applyTextures(pillar, textures);
    pillar.setPosition({5.0f, 0.0f, -2.0f});
    pillar.setScale(glm::vec3(1.0f));
    models_.emplace_back(pillar);
}

void scene::MainScene::addAircraft() {
    ModelManager& modelManager = ModelManager::instance(AssetManager::instance(aAssetManager_));
    auto aircraft = modelManager.getModelFromPath("model/aircraft/piper.obj");
    auto textures = std::map<ndk_helper::mesh::TextureType, std::string>{
        {ndk_helper::mesh::TextureType::DIFFUSE, "model/aircraft/piper_diffuse.jpg"},
        {ndk_helper::mesh::TextureType::NORMAL, "model/aircraft/piper_diffuse.jpg"}
    };

    modelManager.applyTextures(aircraft, textures);
    aircraft.setPosition({0.0f, 0.0f, -1.0f});
    aircraft.setScale(glm::vec3(1.0f));
    models_.emplace_back(aircraft);
}

void scene::MainScene::destroyUniformBuffers() {
    glDeleteBuffers(1, &uboMatrices_);
}

void scene::MainScene::initLights() {
    addPointLights();
    addDirectionalLights();
}

void scene::MainScene::addPointLights() const {
    pillarShader_->activate();
    pillarShader_->set_vec3("pointLight.position", ndk_helper::utils::get_light_dir());
    pillarShader_->set_vec3("pointLight.ambient", glm::vec3{0.2f, 0.2f, 0.2f});
    pillarShader_->set_vec3("pointLight.diffuse", ndk_helper::utils::get_light_color());
    pillarShader_->set_vec3("pointLight.specular", glm::vec3{1.0f, 1.0f, 1.0f});
    pillarShader_->set_float("pointLight.constant", 1.0f);
    pillarShader_->set_float("pointLight.linear", 0.09f);
    pillarShader_->set_float("pointLight.quadratic", 0.032f);
}

void scene::MainScene::addDirectionalLights() const {
    pillarShader_->activate();
    pillarShader_->set_vec3("dirLight.direction", glm::vec3{1.0f, -2.0f, -1.0f});
    pillarShader_->set_vec3("dirLight.ambient", glm::vec3{0.05f, 0.05f, 0.05f});
    pillarShader_->set_vec3("dirLight.diffuse", glm::vec3{0.4f, 0.4f, 0.4f});
    pillarShader_->set_vec3("dirLight.specular", glm::vec3{0.5f, 0.5f, 0.5f});
}

void scene::MainScene::renderOutline(Model& model) const {
    float scaled = 1.05f;
    glm::mat4 modelMat =
        glm::translate(identityMat_, model.getPosition())
            * glm::rotate(
                identityMat_,
                glm::radians(timeManager_.getDeltaTotal() * (-50.0f)),
                glm::vec3{0.0f, 1.0f, 0.0f}
            )
            * glm::scale(identityMat_, scaled*(model.getScale()));

    glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
    glStencilMask(0x00);
    glDisable(GL_DEPTH_TEST);
    outlineShader_->activate();
    outlineShader_->set_mat4("MODEL", modelMat);
    outlineShader_->set_vec3("viewPos", glm::vec3{0.0f, 0.0f, 0.0f});
    model.draw(*(outlineShader_.get()));
    glEnable(GL_DEPTH_TEST);
    glStencilMask(0xFF);
    glStencilFunc(GL_ALWAYS, 0, 0xFF);
}

void scene::MainScene::initSkybox() {
    std::vector<std::string> skyboxFaces = {
        "texture/cubemap/px.png",
        "texture/cubemap/nx.png",
        "texture/cubemap/py.png",
        "texture/cubemap/ny.png",
        "texture/cubemap/pz.png",
        "texture/cubemap/nz.png",
    };
    skybox_ = ndk_helper::utils::make_unique<Skybox>(skyboxFaces, skyboxShader_.get());
    skybox_->onCreate(aAssetManager_);
}
