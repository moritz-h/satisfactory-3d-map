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

Satisfactory3DMap::ModelRenderer::ModelRenderer() {
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

    // First entry in list for fallback
    models_.emplace_back(ModelPath("models/cube.glb", {""}));

    models_.emplace_back(ModelPath("models/foundation_8x4.glb",
        {"/Game/FactoryGame/Buildable/Building/Foundation/Build_Foundation_8x4_01.Build_Foundation_8x4_01_C"}));
    models_.emplace_back(ModelPath("models/foundation_8x2.glb",
        {"/Game/FactoryGame/Buildable/Building/Foundation/Build_Foundation_8x2_01.Build_Foundation_8x2_01_C"}));
    models_.emplace_back(ModelPath("models/foundation_8x1.glb",
        {"/Game/FactoryGame/Buildable/Building/Foundation/Build_Foundation_8x1_01.Build_Foundation_8x1_01_C"}));
    models_.emplace_back(ModelPath(
        "models/ramp_8x4.glb", {"/Game/FactoryGame/Buildable/Building/Ramp/Build_Ramp_8x4_01.Build_Ramp_8x4_01_C"}));
    models_.emplace_back(ModelPath(
        "models/ramp_8x2.glb", {"/Game/FactoryGame/Buildable/Building/Ramp/Build_Ramp_8x2_01.Build_Ramp_8x2_01_C"}));
    models_.emplace_back(ModelPath(
        "models/ramp_8x1.glb", {"/Game/FactoryGame/Buildable/Building/Ramp/Build_Ramp_8x1_01.Build_Ramp_8x1_01_C"}));
    models_.emplace_back(ModelPath("models/doubleramp_8x4.glb",
        {"/Game/FactoryGame/Buildable/Building/Ramp/Build_Ramp_8x8x8.Build_Ramp_8x8x8_C"}));
    models_.emplace_back(ModelPath("models/doubleramp_8x2.glb",
        {"/Game/FactoryGame/Buildable/Building/Ramp/Build_RampDouble.Build_RampDouble_C"}));
    models_.emplace_back(ModelPath("models/doubleramp_8x1.glb",
        {"/Game/FactoryGame/Buildable/Building/Ramp/Build_RampDouble_8x1.Build_RampDouble_8x1_C"}));
    models_.emplace_back(ModelPath(
        "models/wall.glb", {"/Game/FactoryGame/Buildable/Building/Wall/Build_Wall_8x4_01.Build_Wall_8x4_01_C"}));

    splineModels_.emplace_back(ModelPath("models/spline_mesh/conveyor_belt.glb",
        {
            "/Game/FactoryGame/Buildable/Factory/ConveyorBeltMk1/Build_ConveyorBeltMk1.Build_ConveyorBeltMk1_C",
            "/Game/FactoryGame/Buildable/Factory/ConveyorBeltMk2/Build_ConveyorBeltMk2.Build_ConveyorBeltMk2_C",
            "/Game/FactoryGame/Buildable/Factory/ConveyorBeltMk3/Build_ConveyorBeltMk3.Build_ConveyorBeltMk3_C",
            "/Game/FactoryGame/Buildable/Factory/ConveyorBeltMk4/Build_ConveyorBeltMk4.Build_ConveyorBeltMk4_C",
            "/Game/FactoryGame/Buildable/Factory/ConveyorBeltMk5/Build_ConveyorBeltMk5.Build_ConveyorBeltMk5_C",
        }));
    splineModels_.emplace_back(ModelPath("models/spline_mesh/pipe.glb",
        {
            "/Game/FactoryGame/Buildable/Factory/Pipeline/Build_Pipeline.Build_Pipeline_C",
            "/Game/FactoryGame/Buildable/Factory/PipelineMk2/Build_PipelineMK2.Build_PipelineMK2_C",
            "/Game/FactoryGame/Buildable/Factory/PipeHyper/Build_PipeHyper.Build_PipeHyper_C",
        }));
    splineModels_.emplace_back(ModelPath("models/spline_mesh/track.glb",
        {
            "/Game/FactoryGame/Buildable/Factory/Train/Track/Build_RailroadTrack.Build_RailroadTrack_C",
            "/Game/FactoryGame/Buildable/Factory/Train/Track/"
            "Build_RailroadTrackIntegrated.Build_RailroadTrackIntegrated_C",
        }));
}

void Satisfactory3DMap::ModelRenderer::loadSave(const Satisfactory3DMap::SaveGame& saveGame) {
    modelDataList_.clear();
    modelDataList_.resize(models_.size());
    std::vector<std::vector<int32_t>> ids(models_.size());
    std::vector<std::vector<glm::mat4>> transformations(models_.size());

    splineModelDataList_.clear();
    splineModelDataList_.resize(splineModels_.size());
    std::vector<std::vector<SplineSegmentGpu>> splineSegments(splineModels_.size());
    std::vector<std::vector<SplineMeshInstanceGpu>> splineInstances(splineModels_.size());

    for (const auto& obj : saveGame.saveObjects()) {
        if (obj->type() == 1) {
            const auto* actor = dynamic_cast<SaveActor*>(obj.get());
            const auto& className = actor->className();

            int32_t idx = -1;
            for (int32_t i = 0; i < splineModels_.size(); i++) {
                for (int32_t j = 0; j < splineModels_[i].savePaths[j].size(); j++) {
                    if (className == splineModels_[i].savePaths[j]) {
                        idx = i;
                        break;
                    }
                }
            }

            if (idx >= 0) {
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

                continue;
            }

            // Set to idx = 0 as fallback model, start search with i = 1.
            idx = 0;
            for (int32_t i = 1; i < models_.size(); i++) {
                for (int32_t j = 0; j < models_[i].savePaths.size(); j++) {
                    if (className == models_[i].savePaths[j]) {
                        idx = i;
                        break;
                    }
                }
            }

            ids[idx].push_back(actor->id());
            transformations[idx].push_back(actor->transformation());
            modelDataList_[idx].numActors++;
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

void Satisfactory3DMap::ModelRenderer::render(const glm::mat4& projMx, const glm::mat4& viewMx) {
    shader_->use();
    shader_->setUniform("projMx", projMx);
    shader_->setUniform("viewMx", viewMx);

    glActiveTexture(GL_TEXTURE0);
    shader_->setUniform("tex", 0);

    if (models_.size() == modelDataList_.size()) {
        for (std::size_t i = 0; i < models_.size(); i++) {
            const auto& model = models_[i];
            const auto& modelData = modelDataList_[i];

            if (modelData.idBuffer != nullptr && modelData.transformBuffer != nullptr) {
                shader_->setUniform("modelMx", model.model->modelMx());
                shader_->setUniform("normalMx", glm::inverseTranspose(glm::mat3(model.model->modelMx())));
                model.model->bindTexture();
                modelData.idBuffer->bind(0);
                modelData.transformBuffer->bind(1);
                model.model->draw(modelData.numActors);
            }
        }
    }

    glBindTexture(GL_TEXTURE_2D, 0);

    splineShader_->use();
    splineShader_->setUniform("projMx", projMx);
    splineShader_->setUniform("viewMx", viewMx);

    glActiveTexture(GL_TEXTURE0);
    splineShader_->setUniform("tex", 0);

    if (splineModels_.size() == splineModelDataList_.size()) {
        for (std::size_t i = 0; i < splineModels_.size(); i++) {
            const auto& model = splineModels_[i];
            const auto& modelData = splineModelDataList_[i];

            if (modelData.splineSegments != nullptr && modelData.instanceData != nullptr) {
                model.model->bindTexture();
                modelData.splineSegments->bind(0);
                modelData.instanceData->bind(1);
                model.model->draw(modelData.numInstances);
            }
        }
    }

    glUseProgram(0);
}
