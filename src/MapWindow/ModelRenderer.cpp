#include "ModelRenderer.h"

#include <glm/gtc/matrix_inverse.hpp>

#include "SaveGame/Objects/SaveActor.h"
#include "Utils/ResourceUtils.h"

Satisfactory3DMap::ModelRenderer::ModelRenderer() {
    try {
        shader_ = std::make_unique<glowl::GLSLProgram>(glowl::GLSLProgram::ShaderSourceList{
            {glowl::GLSLProgram::ShaderType::Vertex, getStringResource("shaders/model.vert")},
            {glowl::GLSLProgram::ShaderType::Fragment, getStringResource("shaders/model.frag")}});
    } catch (glowl::GLSLProgramException& e) { std::cerr << e.what() << std::endl; }

    // First entry in list for fallback
    models_.emplace_back(ModelPath("models/cube.glb", ""));

    models_.emplace_back(ModelPath("models/foundation_8x4.glb",
        "/Game/FactoryGame/Buildable/Building/Foundation/Build_Foundation_8x4_01.Build_Foundation_8x4_01_C"));
    models_.emplace_back(ModelPath("models/foundation_8x2.glb",
        "/Game/FactoryGame/Buildable/Building/Foundation/Build_Foundation_8x2_01.Build_Foundation_8x2_01_C"));
    models_.emplace_back(ModelPath("models/foundation_8x1.glb",
        "/Game/FactoryGame/Buildable/Building/Foundation/Build_Foundation_8x1_01.Build_Foundation_8x1_01_C"));
}

void Satisfactory3DMap::ModelRenderer::loadSave(const Satisfactory3DMap::SaveGame& saveGame) {
    modelDataList_.clear();
    modelDataList_.resize(models_.size());
    std::vector<std::vector<int32_t>> ids(models_.size());
    std::vector<std::vector<glm::mat4>> transformations(models_.size());

    for (const auto& obj : saveGame.saveObjects()) {
        if (obj->type() == 1) {
            const auto* actor = dynamic_cast<SaveActor*>(obj.get());

            std::size_t idx = 0;
            for (std::size_t i = 1; i < models_.size(); i++) {
                if (actor->className() == models_[i].savePath) {
                    idx = i;
                }
            }

            ids[idx].push_back(actor->id());
            transformations[idx].push_back(actor->transformation());
            modelDataList_[idx].numActors++;
        }
    }

    for (std::size_t i = 0; i < modelDataList_.size(); i++) {
        auto& modelData = modelDataList_[i];
        modelData.idBuffer = std::make_unique<glowl::BufferObject>(GL_SHADER_STORAGE_BUFFER, ids[i]);
        modelData.transformBuffer = std::make_unique<glowl::BufferObject>(GL_SHADER_STORAGE_BUFFER,
            glm::value_ptr(transformations[i].front()), transformations[i].size() * sizeof(glm::mat4), GL_DYNAMIC_DRAW);
    }
}

void Satisfactory3DMap::ModelRenderer::render(const glm::mat4& projMx, const glm::mat4& viewMx) {
    shader_->use();
    shader_->setUniform("projMx", projMx);
    shader_->setUniform("viewMx", viewMx);
    shader_->setUniform("invViewMx", glm::inverse(viewMx));

    glActiveTexture(GL_TEXTURE0);
    shader_->setUniform("tex", 0);

    if (models_.size() == modelDataList_.size()) {
        for (std::size_t i = 0; i < models_.size(); i++) {
            const auto& model = models_[i];
            const auto& modelData = modelDataList_[i];

            shader_->setUniform("modelMx", model.model->modelMx());
            shader_->setUniform("normalMx", glm::inverseTranspose(glm::mat3(model.model->modelMx())));
            model.model->bindTexture();
            modelData.idBuffer->bind(0);
            modelData.transformBuffer->bind(1);
            model.model->draw(modelData.numActors);
        }
    }

    glBindTexture(GL_TEXTURE_2D, 0);
    glUseProgram(0);
}
