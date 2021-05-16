#include "ModelRenderer.h"

#include <iostream>
#include <stdexcept>

#include <glm/gtc/matrix_inverse.hpp>

#include "SaveGame/Objects/SaveActor.h"
#include "SplineData.h"
#include "Utils/ResourceUtils.h"

namespace {
    struct SplineMeshInstanceGpu {
        int32_t id;
        int32_t offset0;
        int32_t offset1;
        int32_t _padding1_;
        float t0;
        float t1;
        float _padding2_[2];
    };
    static_assert(sizeof(SplineMeshInstanceGpu) == 4 * sizeof(int32_t) + 4 * sizeof(float),
        "SplineMeshInstanceGpu: Alignment issue!");
}; // namespace

Satisfactory3DMap::ModelRenderer::ModelRenderer() : wireframe_(false) {
    manager_ = std::make_unique<ModelManager>();

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

void Satisfactory3DMap::ModelRenderer::loadSave(const Satisfactory3DMap::SaveGame& saveGame) {
    modelDataList_.clear();
    modelDataList_.resize(manager_->models().size());
    std::vector<std::vector<int32_t>> ids(manager_->models().size());
    std::vector<std::vector<glm::mat4>> transformations(manager_->models().size());

    splineModelDataList_.clear();
    splineModelDataList_.resize(manager_->splineModels().size());
    std::vector<std::vector<SplineSegmentGpu>> splineSegments(manager_->splineModels().size());
    std::vector<std::vector<SplineMeshInstanceGpu>> splineInstances(manager_->splineModels().size());

    for (const auto& obj : saveGame.saveObjects()) {
        if (obj->type() == 1) {
            const auto* actor = dynamic_cast<SaveActor*>(obj.get());

            const auto& [modelType, idx] = manager_->classifyActor(*actor);

            if (modelType == ModelManager::ModelType::None) {
                // do nothing
            } else if (modelType == ModelManager::ModelType::Model) {
                ids[idx].push_back(actor->id());
                transformations[idx].push_back(actor->transformation());
                modelDataList_[idx].numActors++;
            } else if (modelType == ModelManager::ModelType::SplineModel) {

                SplineData s(*actor);

                // Copy spline segments to spline segment buffer, save position before and after
                int32_t offset0 = static_cast<int32_t>(splineSegments[idx].size());
                for (const auto& splineSegment : s.splineSegments()) {
                    splineSegments[idx].push_back(splineSegment);
                }
                int32_t offset1 = static_cast<int32_t>(splineSegments[idx].size());

                // Calculate number of instances based on meshLength and lengthSum of all spline segments.
                // Round to nearest int, but at least one instance.
                float meshLength = 2.0f; // TODO depends on mesh
                int32_t numInstances = std::max(static_cast<int32_t>(std::lround(s.length() / meshLength)), 1);

                for (int32_t i = 0; i < numInstances; i++) {
                    float t0 = s.length() * static_cast<float>(i) / static_cast<float>(numInstances);
                    float t1 = s.length() * static_cast<float>(i + 1) / static_cast<float>(numInstances);

                    SplineMeshInstanceGpu instance;
                    instance.id = actor->id();
                    instance.offset0 = offset0;
                    instance.offset1 = offset1;
                    instance.t0 = t0;
                    instance.t1 = t1;
                    splineInstances[idx].push_back(instance);
                    splineModelDataList_[idx].numInstances++;
                }
            }
        }
    }

    for (std::size_t i = 0; i < modelDataList_.size(); i++) {
        auto& modelData = modelDataList_[i];
        modelData.idBuffer.reset();
        modelData.transformBuffer.reset();
        if (!ids[i].empty()) {
            modelData.idBuffer = std::make_unique<glowl::BufferObject>(GL_SHADER_STORAGE_BUFFER, ids[i]);
            modelData.transformBuffer = std::make_unique<glowl::BufferObject>(GL_SHADER_STORAGE_BUFFER,
                glm::value_ptr(transformations[i].front()), transformations[i].size() * sizeof(glm::mat4),
                GL_DYNAMIC_DRAW);
        }
    }

    for (std::size_t i = 0; i < splineModelDataList_.size(); i++) {
        auto& modelData = splineModelDataList_[i];
        modelData.splineSegments.reset();
        modelData.instanceData.reset();
        if (!splineSegments[i].empty() && !splineInstances[i].empty()) {
            modelData.splineSegments =
                std::make_unique<glowl::BufferObject>(GL_SHADER_STORAGE_BUFFER, splineSegments[i]);
            modelData.instanceData =
                std::make_unique<glowl::BufferObject>(GL_SHADER_STORAGE_BUFFER, splineInstances[i]);
        }
    }
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

    const auto& modelCount = manager_->models().size();
    if (modelCount == modelDataList_.size()) {
        for (std::size_t i = 0; i < modelCount; i++) {
            const auto& model = manager_->models()[i];
            const auto& modelData = modelDataList_[i];

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

    const auto& splineModelCount = manager_->splineModels().size();
    if (splineModelCount == splineModelDataList_.size()) {
        for (std::size_t i = 0; i < splineModelCount; i++) {
            const auto& model = manager_->splineModels()[i];
            const auto& modelData = splineModelDataList_[i];

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
