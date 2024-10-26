#include "ModelRenderer.h"

#include <iostream>
#include <stdexcept>

#include <glm/gtc/matrix_inverse.hpp>
#include <spdlog/spdlog.h>

#include "Utils/ResourceUtils.h"

Satisfactory3DMap::ModelRenderer::ModelRenderer(const std::shared_ptr<Configuration>& config,
    std::shared_ptr<DataView> dataView)
    : dataView_(std::move(dataView)) {

    wireframeSetting_ = BoolSetting::create("Models wireframe", false);
    config->registerSetting(wireframeSetting_);

    try {
        pakShader_ = std::make_unique<glowl::GLSLProgram>(glowl::GLSLProgram::ShaderSourceList{
            {glowl::GLSLProgram::ShaderType::Vertex, getStringResource("shaders/model_pak.vert")},
            {glowl::GLSLProgram::ShaderType::Fragment, getStringResource("shaders/model_pak.frag")}});
    } catch (glowl::GLSLProgramException& e) {
        spdlog::error(e.what());
    }

    try {
        shader_ = std::make_unique<glowl::GLSLProgram>(glowl::GLSLProgram::ShaderSourceList{
            {glowl::GLSLProgram::ShaderType::Vertex, getStringResource("shaders/model.vert")},
            {glowl::GLSLProgram::ShaderType::Fragment, getStringResource("shaders/model.frag")}});
    } catch (glowl::GLSLProgramException& e) {
        spdlog::error(e.what());
    }

    try {
        splineShader_ = std::make_unique<glowl::GLSLProgram>(glowl::GLSLProgram::ShaderSourceList{
            {glowl::GLSLProgram::ShaderType::Vertex, getStringResource("shaders/splinemesh.vert")},
            {glowl::GLSLProgram::ShaderType::Fragment, getStringResource("shaders/splinemesh.frag")}});
    } catch (glowl::GLSLProgramException& e) {
        spdlog::error(e.what());
    }
}

void Satisfactory3DMap::ModelRenderer::render(const glm::mat4& projMx, const glm::mat4& viewMx, int selectedId) {
    if (wireframeSetting_->getVal()) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }

    dataView_->actorIdBuffer()->bind(0);
    dataView_->actorTransformationBuffer()->bind(1);

    pakShader_->use();
    pakShader_->setUniform("projMx", projMx);
    pakShader_->setUniform("viewMx", viewMx);
    pakShader_->setUniform("selectedId", selectedId);

    if (!dataView_->pakModelDataList().empty()) {
        for (std::size_t i = 0; i < dataView_->pakModelDataList().size(); i++) {
            const auto& model = dataView_->manager()->pakModels()[i];
            const auto& modelData = dataView_->pakModelDataList()[i];

            if (modelData.listOffsetBuffer != nullptr) {
                modelData.listOffsetBuffer->bind(2);
                for (const auto& meshInfo : model) {
                    pakShader_->setUniform("modelMx", meshInfo.transform);
                    pakShader_->setUniform("normalMx", glm::inverseTranspose(glm::mat3(meshInfo.transform)));
                    meshInfo.mesh->draw(modelData.numActors);
                }
            }
        }
    }

    shader_->use();
    shader_->setUniform("projMx", projMx);
    shader_->setUniform("viewMx", viewMx);
    shader_->setUniform("selectedId", selectedId);

    glActiveTexture(GL_TEXTURE0);
    shader_->setUniform("tex", 0);

    if (!dataView_->modelDataList().empty()) {
        for (std::size_t i = 0; i < dataView_->modelDataList().size(); i++) {
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

    if (!dataView_->splineModelDataList().empty()) {
        for (std::size_t i = 0; i < dataView_->splineModelDataList().size(); i++) {
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

    if (wireframeSetting_->getVal()) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }
}
