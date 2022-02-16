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

            if (modelData.idBuffer != nullptr && modelData.transformBuffer != nullptr) {
                shader_->setUniform("modelMx", model->modelMx());
                shader_->setUniform("normalMx", glm::inverseTranspose(glm::mat3(model->modelMx())));
                model->bindTexture();
                modelData.idBuffer->bind(0);
                modelData.transformBuffer->bind(1);
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
                modelData.splineSegments->bind(0);
                modelData.instanceData->bind(1);
                model->draw(modelData.numInstances);
            }
        }
    }

    glUseProgram(0);

    if (wireframe_) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }
}
