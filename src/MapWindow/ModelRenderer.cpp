#include "ModelRenderer.h"

#include <iostream>
#include <stdexcept>

#include <glm/gtc/matrix_inverse.hpp>

#include "GameTypes/SaveObjects/SaveActor.h"
#include "Utils/ResourceUtils.h"

Satisfactory3DMap::ModelRenderer::ModelRenderer(std::shared_ptr<DataView> dataView)
    : dataView_(std::move(dataView)),
      wireframe_(false) {
    try {
        pakShader_ = std::make_unique<glowl::GLSLProgram>(glowl::GLSLProgram::ShaderSourceList{
            {glowl::GLSLProgram::ShaderType::Vertex, getStringResource("shaders/model_pak.vert")},
            {glowl::GLSLProgram::ShaderType::Fragment, getStringResource("shaders/model_pak.frag")}});
    } catch (glowl::GLSLProgramException& e) { std::cerr << e.what() << std::endl; }

    try {
        shader_ = std::make_unique<glowl::GLSLProgram>(glowl::GLSLProgram::ShaderSourceList{
            {glowl::GLSLProgram::ShaderType::Vertex, getStringResource("shaders/model.vert")},
            {glowl::GLSLProgram::ShaderType::Fragment, getStringResource("shaders/model.frag")}});
    } catch (glowl::GLSLProgramException& e) { std::cerr << e.what() << std::endl; }

    try {
        splineShader_ = std::make_unique<glowl::GLSLProgram>(glowl::GLSLProgram::ShaderSourceList{
            {glowl::GLSLProgram::ShaderType::Vertex, getStringResource("shaders/splinemesh.vert")},
            {glowl::GLSLProgram::ShaderType::Fragment, getStringResource("shaders/splinemesh.frag")}});
    } catch (glowl::GLSLProgramException& e) { std::cerr << e.what() << std::endl; }
}

void Satisfactory3DMap::ModelRenderer::render(const glm::mat4& projMx, const glm::mat4& viewMx, int selectedId) {
    if (wireframe_) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }

    dataView_->actorIdBuffer()->bind(0);
    dataView_->actorTransformationBuffer()->bind(1);

    pakShader_->use();
    pakShader_->setUniform("projMx", projMx);
    pakShader_->setUniform("viewMx", viewMx);
    pakShader_->setUniform("selectedId", selectedId);

    const auto& pakModelCount = dataView_->manager()->pakModels().size();
    if (pakModelCount == dataView_->pakModelDataList().size()) {
        for (std::size_t i = 0; i < pakModelCount; i++) {
            const auto& model = dataView_->manager()->pakModels()[i];
            const auto& modelData = dataView_->pakModelDataList()[i];

            if (modelData.listOffsetBuffer != nullptr) {
                const auto& modelMx = dataView_->manager()->pakTransformations()[i];
                pakShader_->setUniform("modelMx", modelMx);
                pakShader_->setUniform("normalMx", glm::inverseTranspose(glm::mat3(modelMx)));

                modelData.listOffsetBuffer->bind(2);
                model->draw(modelData.numActors);
            }
        }
    }

    shader_->use();
    shader_->setUniform("projMx", projMx);
    shader_->setUniform("viewMx", viewMx);
    shader_->setUniform("selectedId", selectedId);

    glActiveTexture(GL_TEXTURE0);
    shader_->setUniform("tex", 0);

    const auto& modelCount = dataView_->manager()->models().size();
    if (modelCount == dataView_->modelDataList().size()) {
        for (std::size_t i = 0; i < modelCount; i++) {
            const auto& model = dataView_->manager()->models()[i];
            const auto& modelData = dataView_->modelDataList()[i];

            if (modelData.listOffsetBuffer != nullptr) {
                shader_->setUniform("modelMx", model->modelMx());
                shader_->setUniform("normalMx", glm::inverseTranspose(glm::mat3(model->modelMx())));
                model->bindTexture();
                modelData.listOffsetBuffer->bind(2);
                model->draw(modelData.numActors);
            }
        }
    }

    glBindTexture(GL_TEXTURE_2D, 0);

    splineShader_->use();
    splineShader_->setUniform("projMx", projMx);
    splineShader_->setUniform("viewMx", viewMx);
    splineShader_->setUniform("selectedId", selectedId);

    glActiveTexture(GL_TEXTURE0);
    splineShader_->setUniform("tex", 0);

    const auto& splineModelCount = dataView_->manager()->splineModels().size();
    if (splineModelCount == dataView_->splineModelDataList().size()) {
        for (std::size_t i = 0; i < splineModelCount; i++) {
            const auto& model = dataView_->manager()->splineModels()[i];
            const auto& modelData = dataView_->splineModelDataList()[i];

            if (modelData.splineSegments != nullptr && modelData.instanceData != nullptr) {
                model->bindTexture();
                modelData.splineSegments->bind(2);
                modelData.instanceData->bind(3);
                model->draw(modelData.numInstances);
            }
        }
    }

    glUseProgram(0);

    if (wireframe_) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }
}
