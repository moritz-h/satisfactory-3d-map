#include "ModelRenderer.h"

#include <glm/gtc/matrix_inverse.hpp>

#include "SaveGame/Objects/SaveActor.h"
#include "SaveGame/Types/Arrays/StructArray.h"
#include "SaveGame/Types/Properties/ArrayProperty.h"
#include "SaveGame/Types/Properties/StructProperty.h"
#include "SaveGame/Types/Structs/PropertyStruct.h"
#include "SaveGame/Types/Structs/VectorStruct.h"
#include "Utils/ResourceUtils.h"

namespace {
    struct SplinePointData {
        glm::vec3 location;
        glm::vec3 arriveTangent;
        glm::vec3 leaveTangent;
    };

    struct SplineSegmentGpu {
        glm::vec4 p0;
        glm::vec4 p1;
        glm::vec4 tangent0;
        glm::vec4 tangent1;
    };
    static_assert(sizeof(SplineSegmentGpu) == 16 * sizeof(float), "SplineSegmentGpu: Alignment issue!");

    Satisfactory3DMap::ArrayProperty& getSplineDataProperty(
        const std::vector<std::unique_ptr<Satisfactory3DMap::Property>>& properties) {
        for (const auto& p : properties) {
            if (p->name() == "mSplineData" && p->type() == "ArrayProperty") {
                return dynamic_cast<Satisfactory3DMap::ArrayProperty&>(*p);
            }
        }

        throw std::runtime_error("mSplineData property missing!");
    }

    std::vector<SplinePointData> getSplineData(const Satisfactory3DMap::SaveActor& a) {
        auto& ap = getSplineDataProperty(a.properties());
        if (ap.arrayType() != "StructProperty") {
            throw std::runtime_error("Expect StructProperty!");
        }

        auto& sa = dynamic_cast<Satisfactory3DMap::StructArray&>(*ap.array());
        if (sa.structName() != "SplinePointData") {
            throw std::runtime_error("Expect SplinePointData!");
        }

        std::vector<SplinePointData> result;
        for (auto& s : sa.array()) {
            auto& ps = dynamic_cast<Satisfactory3DMap::PropertyStruct&>(*s);
            if (ps.properties().size() != 3) {
                throw std::runtime_error("Unexpected struct size!");
            }
            auto& locationStruct = *dynamic_cast<Satisfactory3DMap::StructProperty&>(*ps.properties()[0]).value();
            auto& arriveTangentStruct = *dynamic_cast<Satisfactory3DMap::StructProperty&>(*ps.properties()[1]).value();
            auto& leaveTangentStruct = *dynamic_cast<Satisfactory3DMap::StructProperty&>(*ps.properties()[2]).value();

            SplinePointData data;
            data.location = dynamic_cast<Satisfactory3DMap::VectorStruct&>(locationStruct).value();
            data.arriveTangent = dynamic_cast<Satisfactory3DMap::VectorStruct&>(arriveTangentStruct).value();
            data.leaveTangent = dynamic_cast<Satisfactory3DMap::VectorStruct&>(leaveTangentStruct).value();
            result.emplace_back(data);
        }
        return result;
    }
} // namespace

Satisfactory3DMap::ModelRenderer::ModelRenderer() : numSplineSegments_(0), splineSubdivision_(5) {
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
    std::vector<int32_t> splineIds;
    std::vector<SplineSegmentGpu> splineSegments;

    for (const auto& obj : saveGame.saveObjects()) {
        if (obj->type() == 1) {
            const auto* actor = dynamic_cast<SaveActor*>(obj.get());
            const auto& className = actor->className();

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
                auto splines = getSplineData(*actor);
                for (std::size_t i = 1; i < splines.size(); i++) {
                    auto p0 = splines[i - 1].location * glm::vec3(0.01f, -0.01f, 0.01f);
                    auto p1 = splines[i].location * glm::vec3(0.01f, -0.01f, 0.01f);
                    auto t0 = splines[i - 1].leaveTangent * glm::vec3(0.01f, -0.01f, 0.01f);
                    auto t1 = splines[i].arriveTangent * glm::vec3(0.01f, -0.01f, 0.01f);

                    SplineSegmentGpu segment;
                    segment.p0 = (glm::translate(glm::mat4(1.0f), p0) * actor->transformation())[3];
                    segment.p1 = (glm::translate(glm::mat4(1.0f), p1) * actor->transformation())[3];
                    segment.tangent0 = glm::vec4(t0, 0.0f);
                    segment.tangent1 = glm::vec4(t1, 0.0f);

                    splineIds.emplace_back(actor->id());
                    splineSegments.emplace_back(segment);
                    numSplineSegments_++;
                }
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
        modelData.idBuffer = std::make_unique<glowl::BufferObject>(GL_SHADER_STORAGE_BUFFER, ids[i]);
        modelData.transformBuffer = std::make_unique<glowl::BufferObject>(GL_SHADER_STORAGE_BUFFER,
            glm::value_ptr(transformations[i].front()), transformations[i].size() * sizeof(glm::mat4), GL_DYNAMIC_DRAW);
    }

    splineIds_ = std::make_unique<glowl::BufferObject>(GL_SHADER_STORAGE_BUFFER, splineIds);
    splineSegments_ = std::make_unique<glowl::BufferObject>(GL_SHADER_STORAGE_BUFFER, splineSegments);
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

    splineShader_->use();
    splineShader_->setUniform("projMx", projMx);
    splineShader_->setUniform("viewMx", viewMx);
    splineShader_->setUniform("splineSubdivision", splineSubdivision_);

    splineIds_->bind(0);
    splineSegments_->bind(1);

    glBindVertexArray(vaEmpty_);
    glDrawArraysInstanced(GL_POINTS, 0, 1, numSplineSegments_ * splineSubdivision_);
    glBindVertexArray(0);

    glUseProgram(0);
}
