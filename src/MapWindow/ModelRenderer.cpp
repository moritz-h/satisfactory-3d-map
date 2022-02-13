#include "ModelRenderer.h"

#include <iostream>
#include <stdexcept>

#include <glm/gtc/matrix_inverse.hpp>

#include "GameTypes/SaveObjects/SaveActor.h"
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
        glm::vec4 forward;
    };
    static_assert(sizeof(SplineMeshInstanceGpu) == 4 * sizeof(int32_t) + 8 * sizeof(float),
        "SplineMeshInstanceGpu: Alignment issue!");

    glm::vec3 deCasteljau(const Satisfactory3DMap::SplineSegmentGpu& segment, float t) {
        // Bezier control points
        const glm::vec3 b0 = glm::vec3(segment.p0);
        const glm::vec3 b3 = glm::vec3(segment.p1);
        // Unreal Engine seems to use cubic Hermite splines, convert to Bezier control points.
        const glm::vec3 b1 = b0 + glm::vec3(segment.tangent0) / 3.0f;
        const glm::vec3 b2 = b3 - glm::vec3(segment.tangent1) / 3.0f;

        // De Casteljau
        const glm::vec3 b01 = glm::mix(b0, b1, t);
        const glm::vec3 b11 = glm::mix(b1, b2, t);
        const glm::vec3 b21 = glm::mix(b2, b3, t);

        const glm::vec3 b02 = glm::mix(b01, b11, t);
        const glm::vec3 b12 = glm::mix(b11, b21, t);

        return glm::mix(b02, b12, t);
    }

    glm::vec3 determineInstanceForward(float t0, float t1,
        const std::vector<Satisfactory3DMap::SplineSegmentGpu>& splineSegments, int32_t offset0, int32_t offset1) {
        // Determine spline segments
        int segmentIdx0 = offset0;
        int segmentIdx1 = offset0;
        while (t0 > splineSegments[segmentIdx0].length && segmentIdx0 < offset1 - 1) {
            t0 -= splineSegments[segmentIdx0].length;
            t1 -= splineSegments[segmentIdx1].length;
            segmentIdx0++;
            segmentIdx1++;
        }
        while (t1 > splineSegments[segmentIdx1].length && segmentIdx1 < offset1 - 1) {
            t1 -= splineSegments[segmentIdx1].length;
            segmentIdx1++;
        }
        const Satisfactory3DMap::SplineSegmentGpu& segment0 = splineSegments[segmentIdx0];
        const Satisfactory3DMap::SplineSegmentGpu& segment1 = splineSegments[segmentIdx1];

        const float segmentT0 = std::clamp(t0 / segment0.length, 0.0f, 1.0f);
        const float segmentT1 = std::clamp(t1 / segment1.length, 0.0f, 1.0f);

        glm::vec3 p0 = deCasteljau(segment0, segmentT0);
        glm::vec3 p1 = deCasteljau(segment1, segmentT1);

        // Use vector from start to end of a single instance as approximation of forward vector.
        const glm::vec3 forward = glm::normalize(p1 - p0);
        return forward;
    }
} // namespace

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
                actorBufferPositions_.emplace(actor->id(), std::make_tuple(idx, modelDataList_[idx].numActors));
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

                    const glm::vec3 forward = determineInstanceForward(t0, t1, splineSegments[idx], offset0, offset1);

                    SplineMeshInstanceGpu instance;
                    instance.id = actor->id();
                    instance.offset0 = offset0;
                    instance.offset1 = offset1;
                    instance.t0 = t0;
                    instance.t1 = t1;
                    instance.forward = glm::vec4(forward, 1.0f);
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

void Satisfactory3DMap::ModelRenderer::updateActor(const Satisfactory3DMap::SaveActor& actor) {
    if (actorBufferPositions_.count(actor.id()) > 0) {
        const auto& info = actorBufferPositions_.at(actor.id());
        const auto bufferId = std::get<0>(info);
        const auto bufferPos = std::get<1>(info);
        modelDataList_[bufferId].transformBuffer->bufferSubData(glm::value_ptr(actor.transformation()),
            sizeof(glm::mat4), bufferPos * sizeof(glm::mat4));
    }

    // TODO spline model actors
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
