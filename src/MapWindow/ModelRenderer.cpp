#include "ModelRenderer.h"

#include <iostream>
#include <stdexcept>

#include <glm/gtc/matrix_inverse.hpp>

#include "SaveGame/Objects/SaveActor.h"
#include "SplineData.h"
#include "Utils/ResourceUtils.h"

Satisfactory3DMap::ModelRenderer::ModelRenderer() : numSplineSegments_(0), splineSubdivision_(16) {
    try {
        shader_ = std::make_unique<glowl::GLSLProgram>(glowl::GLSLProgram::ShaderSourceList{
            {glowl::GLSLProgram::ShaderType::Vertex, getStringResource("shaders/model.vert")},
            {glowl::GLSLProgram::ShaderType::Fragment, getStringResource("shaders/model.frag")}});
    } catch (glowl::GLSLProgramException& e) { std::cerr << e.what() << std::endl; }

    try {
        splineShader_ = std::make_unique<glowl::GLSLProgram>(glowl::GLSLProgram::ShaderSourceList{
            {glowl::GLSLProgram::ShaderType::Vertex, getStringResource("shaders/spline.vert")},
            {glowl::GLSLProgram::ShaderType::Geometry, getStringResource("shaders/spline.geom")},
            {glowl::GLSLProgram::ShaderType::Fragment, getStringResource("shaders/spline.frag")}});
    } catch (glowl::GLSLProgramException& e) { std::cerr << e.what() << std::endl; }

    glGenVertexArrays(1, &vaEmpty_);
    // Bind once to create
    glBindVertexArray(vaEmpty_);
    glBindVertexArray(0);

    // First entry in list for fallback
    models_.emplace_back(ModelPath("models/cube.glb", ""));

    models_.emplace_back(ModelPath("models/foundation_8x4.glb",
        "/Game/FactoryGame/Buildable/Building/Foundation/Build_Foundation_8x4_01.Build_Foundation_8x4_01_C"));
    models_.emplace_back(ModelPath("models/foundation_8x2.glb",
        "/Game/FactoryGame/Buildable/Building/Foundation/Build_Foundation_8x2_01.Build_Foundation_8x2_01_C"));
    models_.emplace_back(ModelPath("models/foundation_8x1.glb",
        "/Game/FactoryGame/Buildable/Building/Foundation/Build_Foundation_8x1_01.Build_Foundation_8x1_01_C"));
    models_.emplace_back(ModelPath(
        "models/ramp_8x4.glb", "/Game/FactoryGame/Buildable/Building/Ramp/Build_Ramp_8x4_01.Build_Ramp_8x4_01_C"));
    models_.emplace_back(ModelPath(
        "models/ramp_8x2.glb", "/Game/FactoryGame/Buildable/Building/Ramp/Build_Ramp_8x2_01.Build_Ramp_8x2_01_C"));
    models_.emplace_back(ModelPath(
        "models/ramp_8x1.glb", "/Game/FactoryGame/Buildable/Building/Ramp/Build_Ramp_8x1_01.Build_Ramp_8x1_01_C"));
    models_.emplace_back(ModelPath(
        "models/doubleramp_8x4.glb", "/Game/FactoryGame/Buildable/Building/Ramp/Build_Ramp_8x8x8.Build_Ramp_8x8x8_C"));
    models_.emplace_back(ModelPath(
        "models/doubleramp_8x2.glb", "/Game/FactoryGame/Buildable/Building/Ramp/Build_RampDouble.Build_RampDouble_C"));
    models_.emplace_back(ModelPath("models/doubleramp_8x1.glb",
        "/Game/FactoryGame/Buildable/Building/Ramp/Build_RampDouble_8x1.Build_RampDouble_8x1_C"));
    models_.emplace_back(ModelPath(
        "models/wall.glb", "/Game/FactoryGame/Buildable/Building/Wall/Build_Wall_8x4_01.Build_Wall_8x4_01_C"));
}

void Satisfactory3DMap::ModelRenderer::loadSave(const Satisfactory3DMap::SaveGame& saveGame) {
    modelDataList_.clear();
    modelDataList_.resize(models_.size());
    std::vector<std::vector<int32_t>> ids(models_.size());
    std::vector<std::vector<glm::mat4>> transformations(models_.size());

    numSplineSegments_ = 0;
    std::vector<SplineSegmentGpu> splineSegments;

    for (const auto& obj : saveGame.saveObjects()) {
        if (obj->type() == 1) {
            const auto* actor = dynamic_cast<SaveActor*>(obj.get());
            const auto& className = actor->className();

            SplineModelType spline_type = SplineModelType::None;
            if (className == "/Game/FactoryGame/Buildable/Factory/ConveyorBeltMk1/"
                             "Build_ConveyorBeltMk1.Build_ConveyorBeltMk1_C" ||
                className == "/Game/FactoryGame/Buildable/Factory/ConveyorBeltMk2/"
                             "Build_ConveyorBeltMk2.Build_ConveyorBeltMk2_C" ||
                className == "/Game/FactoryGame/Buildable/Factory/ConveyorBeltMk3/"
                             "Build_ConveyorBeltMk3.Build_ConveyorBeltMk3_C" ||
                className == "/Game/FactoryGame/Buildable/Factory/ConveyorBeltMk4/"
                             "Build_ConveyorBeltMk4.Build_ConveyorBeltMk4_C" ||
                className == "/Game/FactoryGame/Buildable/Factory/ConveyorBeltMk5/"
                             "Build_ConveyorBeltMk5.Build_ConveyorBeltMk5_C") {
                spline_type = SplineModelType::ConveyorBelt;
            } else if (className == "/Game/FactoryGame/Buildable/Factory/Pipeline/Build_Pipeline.Build_Pipeline_C" ||
                       className ==
                           "/Game/FactoryGame/Buildable/Factory/PipelineMk2/Build_PipelineMK2.Build_PipelineMK2_C") {
                spline_type = SplineModelType::Pipe;
            } else if (className == "/Game/FactoryGame/Buildable/Factory/PipeHyper/Build_PipeHyper.Build_PipeHyper_C") {
                spline_type = SplineModelType::Hyper;
            } else if (className == "/Game/FactoryGame/Buildable/Factory/Train/Track/"
                                    "Build_RailroadTrack.Build_RailroadTrack_C" ||
                       className == "/Game/FactoryGame/Buildable/Factory/Train/Track/"
                                    "Build_RailroadTrackIntegrated.Build_RailroadTrackIntegrated_C") {
                spline_type = SplineModelType::Track;
            }

            if (spline_type != SplineModelType::None) {
                SplineData s(spline_type, *actor);

                for (auto splineSegment : s.splineSegments_) {
                    splineSegments.emplace_back(splineSegment);
                }
                numSplineSegments_ += static_cast<int32_t>(s.splineSegments_.size());

                continue;
            }

            std::size_t idx = 0;
            for (std::size_t i = 1; i < models_.size(); i++) {
                if (className == models_[i].savePath) {
                    idx = i;
                    break;
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

    splineSegments_.reset();
    if (numSplineSegments_ > 0) {
        splineSegments_ = std::make_unique<glowl::BufferObject>(GL_SHADER_STORAGE_BUFFER, splineSegments);
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

    if (numSplineSegments_ > 0 && splineSegments_ != nullptr) {
        splineShader_->use();
        splineShader_->setUniform("projMx", projMx);
        splineShader_->setUniform("viewMx", viewMx);
        splineShader_->setUniform("splineSubdivision", splineSubdivision_);

        splineSegments_->bind(0);

        glBindVertexArray(vaEmpty_);
        glDrawArraysInstanced(GL_POINTS, 0, 1, numSplineSegments_ * splineSubdivision_);
        glBindVertexArray(0);
    }

    glUseProgram(0);
}
