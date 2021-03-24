#include "ModelRenderer.h"

#include <glm/gtc/matrix_inverse.hpp>

#include "SaveGame/Objects/SaveActor.h"
#include "Utils/ResourceUtils.h"

Satisfactory3DMap::ModelRenderer::ModelRenderer()
    : numActorsCube_(0), numActorsFoundation8x4_(0), numActorsFoundation8x2_(0), numActorsFoundation8x1_(0) {

    try {
        shader_ = std::make_unique<glowl::GLSLProgram>(glowl::GLSLProgram::ShaderSourceList{
            {glowl::GLSLProgram::ShaderType::Vertex, getStringResource("shaders/model.vert")},
            {glowl::GLSLProgram::ShaderType::Fragment, getStringResource("shaders/model.frag")}});
    } catch (glowl::GLSLProgramException& e) { std::cerr << e.what() << std::endl; }

    modelCube_ = std::make_unique<Model>("models/cube.glb");
    modelFoundation8x4_ = std::make_unique<Model>("models/foundation_8x4.glb");
    modelFoundation8x2_ = std::make_unique<Model>("models/foundation_8x2.glb");
    modelFoundation8x1_ = std::make_unique<Model>("models/foundation_8x1.glb");
}

void Satisfactory3DMap::ModelRenderer::loadSave(const Satisfactory3DMap::SaveGame& saveGame) {
    numActorsCube_ = 0;
    numActorsFoundation8x4_ = 0;
    numActorsFoundation8x2_ = 0;
    numActorsFoundation8x1_ = 0;
    std::vector<float> positionsCube;
    std::vector<float> positionsFoundation8x4;
    std::vector<float> positionsFoundation8x2;
    std::vector<float> positionsFoundation8x1;
    for (const auto& obj : saveGame.saveObjects()) {
        if (obj->type() == 1) {
            const auto* actor = dynamic_cast<SaveActor*>(obj.get());
            const auto& pos = actor->position();
            if (actor->className() ==
                "/Game/FactoryGame/Buildable/Building/Foundation/Build_Foundation_8x4_01.Build_Foundation_8x4_01_C") {
                positionsFoundation8x4.push_back(pos.x / 100.0f);
                positionsFoundation8x4.push_back(-pos.y / 100.0f);
                positionsFoundation8x4.push_back(pos.z / 100.0f);
                positionsFoundation8x4.push_back(0.0f); // std430 vec4 alignment
                numActorsFoundation8x4_++;
            } else if (actor->className() == "/Game/FactoryGame/Buildable/Building/Foundation/"
                                             "Build_Foundation_8x2_01.Build_Foundation_8x2_01_C") {
                positionsFoundation8x2.push_back(pos.x / 100.0f);
                positionsFoundation8x2.push_back(-pos.y / 100.0f);
                positionsFoundation8x2.push_back(pos.z / 100.0f);
                positionsFoundation8x2.push_back(0.0f); // std430 vec4 alignment
                numActorsFoundation8x2_++;
            } else if (actor->className() == "/Game/FactoryGame/Buildable/Building/Foundation/"
                                             "Build_Foundation_8x1_01.Build_Foundation_8x1_01_C") {
                positionsFoundation8x1.push_back(pos.x / 100.0f);
                positionsFoundation8x1.push_back(-pos.y / 100.0f);
                positionsFoundation8x1.push_back(pos.z / 100.0f);
                positionsFoundation8x1.push_back(0.0f); // std430 vec4 alignment
                numActorsFoundation8x1_++;
            } else {
                positionsCube.push_back(pos.x / 100.0f);
                positionsCube.push_back(-pos.y / 100.0f);
                positionsCube.push_back(pos.z / 100.0f);
                positionsCube.push_back(0.0f); // std430 vec4 alignment
                numActorsCube_++;
            }
        }
    }

    posBufferCube_ = std::make_unique<glowl::BufferObject>(GL_SHADER_STORAGE_BUFFER, positionsCube);
    posBufferFoundation8x4_ = std::make_unique<glowl::BufferObject>(GL_SHADER_STORAGE_BUFFER, positionsFoundation8x4);
    posBufferFoundation8x2_ = std::make_unique<glowl::BufferObject>(GL_SHADER_STORAGE_BUFFER, positionsFoundation8x2);
    posBufferFoundation8x1_ = std::make_unique<glowl::BufferObject>(GL_SHADER_STORAGE_BUFFER, positionsFoundation8x1);
}

void Satisfactory3DMap::ModelRenderer::render(const glm::mat4& projMx, const glm::mat4& viewMx) {
    shader_->use();
    shader_->setUniform("projMx", projMx);
    shader_->setUniform("viewMx", viewMx);
    shader_->setUniform("invViewMx", glm::inverse(viewMx));

    glActiveTexture(GL_TEXTURE0);
    shader_->setUniform("tex", 0);

    shader_->setUniform("modelMx", modelCube_->modelMx());
    shader_->setUniform("normalMx", glm::inverseTranspose(glm::mat3(modelCube_->modelMx())));
    modelCube_->bindTexture();
    posBufferCube_->bind(0);
    modelCube_->draw(numActorsCube_);

    shader_->setUniform("modelMx", modelFoundation8x4_->modelMx());
    shader_->setUniform("normalMx", glm::inverseTranspose(glm::mat3(modelFoundation8x4_->modelMx())));
    modelFoundation8x4_->bindTexture();
    posBufferFoundation8x4_->bind(0);
    modelFoundation8x4_->draw(numActorsFoundation8x4_);

    shader_->setUniform("modelMx", modelFoundation8x2_->modelMx());
    shader_->setUniform("normalMx", glm::inverseTranspose(glm::mat3(modelFoundation8x2_->modelMx())));
    modelFoundation8x2_->bindTexture();
    posBufferFoundation8x2_->bind(0);
    modelFoundation8x2_->draw(numActorsFoundation8x2_);

    shader_->setUniform("modelMx", modelFoundation8x1_->modelMx());
    shader_->setUniform("normalMx", glm::inverseTranspose(glm::mat3(modelFoundation8x1_->modelMx())));
    modelFoundation8x1_->bindTexture();
    posBufferFoundation8x1_->bind(0);
    modelFoundation8x1_->draw(numActorsFoundation8x1_);

    glBindTexture(GL_TEXTURE_2D, 0);
    glUseProgram(0);
}
