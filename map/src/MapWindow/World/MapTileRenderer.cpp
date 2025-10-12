#include "MapTileRenderer.h"

#include <glm/gtc/matrix_inverse.hpp>
#include <spdlog/spdlog.h>

#include "SatisfactorySave/GameTypes/Arrays/ObjectArray.h"
#include "SatisfactorySave/GameTypes/Arrays/StructArray.h"
#include "SatisfactorySave/GameTypes/Properties/ArrayProperty.h"
#include "SatisfactorySave/GameTypes/Properties/NameProperty.h"
#include "SatisfactorySave/GameTypes/Properties/ObjectProperty.h"
#include "SatisfactorySave/GameTypes/Properties/StructProperty.h"
#include "SatisfactorySave/GameTypes/Structs/PropertyStruct.h"
#include "SatisfactorySave/GameTypes/UE/Engine/Engine/StaticMesh.h"
#include "SatisfactorySave/GameTypes/UE/Engine/Engine/Texture2D.h"
#include "SatisfactorySave/GameTypes/UE/Engine/GameFramework/Actor.h"

#include "../OpenGL/GlowlFactory.h"
#include "Utils/ResourceUtils.h"

Satisfactory3DMap::MapTileRenderer::MapTileRenderer(const std::shared_ptr<Configuration>& config,
    const std::shared_ptr<SatisfactorySave::PakManager>& pakManager) {

    wireframeSetting_ = BoolSetting::create("Tile wireframe", false);
    config->registerSetting(wireframeSetting_);

    faceNormalsSetting_ = BoolSetting::create("Use face normals", false);
    config->registerSetting(faceNormalsSetting_);

    if (pakManager != nullptr) {
        try {
            if (!pakManager->containsAssetFilename("Game/FactoryGame/Map/GameLevel01/Persistent_Level.umap")) {
                throw std::runtime_error("Missing Persistent_Level asset!");
            }
            auto mapAsset = pakManager->readAsset("Game/FactoryGame/Map/GameLevel01/Persistent_Level.umap");

            auto WPHLODClassIndex = pakManager->tryGetScriptObjectIndex("/Script/Engine/WorldPartitionHLOD");
            if (!WPHLODClassIndex.has_value()) {
                throw std::runtime_error("ClassIndex missing for /Script/Engine/WorldPartitionHLOD!");
            }

            for (std::size_t i = 0; i < mapAsset.exportMap().size(); i++) {
                const auto& exp = mapAsset.exportMap()[i];
                if (exp.ClassIndex != WPHLODClassIndex.value()) {
                    continue;
                }

                const auto WPHLOD = mapAsset.getExportObjectByIdx(i);
                const auto WPHLOD_obj = WPHLOD->cast_object<s::AActor>();

                // TODO WPHLOD_obj->ActorLabel;

                const auto& instCompProp = WPHLOD_obj->Properties.get<s::ArrayProperty>("InstanceComponents");
                const auto instCompArray = std::dynamic_pointer_cast<s::ObjectArray>(instCompProp.Value);
                if (instCompArray == nullptr) {
                    throw std::runtime_error("InstanceComponents array missing!");
                }

                for (const auto& instCompRef : instCompArray->Values) {
                    if (instCompRef.pakValue() <= 0) {
                        continue;
                    }

                    const auto instCompExp = mapAsset.getExportObjectByIdx(instCompRef.pakValue() - 1);
                    const auto& meshRef = instCompExp->Object->Properties.get<s::ObjectProperty>("StaticMesh").Value;
                    if (meshRef.pakValue() <= 0) {
                        continue;
                    }
                    const auto meshExp = mapAsset.getExportObjectByIdx(meshRef.pakValue() - 1);
                    const auto meshObj = meshExp->cast_object<s::UStaticMesh>();
                    if (meshObj == nullptr) {
                        throw std::runtime_error("Invalid UStaticMesh!");
                    }

                    MapTileData mapTile;
                    mapTile.mesh = makeGlowlMesh(*meshObj);

                    mapTile.modelMx = glm::scale(glm::mat4(1.0f), glm::vec3(0.01f));
                    mapTile.normalMx = glm::inverseTranspose(glm::mat3(mapTile.modelMx));

                    const auto& matProp = meshObj->Properties.get<s::ArrayProperty>("StaticMaterials");
                    const auto matArray = std::dynamic_pointer_cast<s::StructArray>(matProp.Value);
                    if (matArray == nullptr || matArray->Values.size() != 1) {
                        throw std::runtime_error("Invalid/Unexpected StaticMaterials array!");
                    }
                    const auto matStruct = std::dynamic_pointer_cast<s::PropertyStruct>(matArray->Values[0]);
                    if (matStruct == nullptr) {
                        throw std::runtime_error("Invalid StaticMaterials PropertyStruct!");
                    }
                    const auto& matInterfaceRef = matStruct->Data.get<s::ObjectProperty>("MaterialInterface").Value;
                    if (matInterfaceRef.pakValue() <= 0) {
                        throw std::runtime_error("Invalid MaterialInterface reference!");
                    }

                    const auto matExp = mapAsset.getExportObjectByIdx(matInterfaceRef.pakValue() - 1);
                    const auto& texParamProp =
                        matExp->Object->Properties.get<s::ArrayProperty>("TextureParameterValues");
                    const auto texParamArray = std::dynamic_pointer_cast<s::StructArray>(texParamProp.Value);
                    if (texParamArray == nullptr || texParamArray->Values.size() != 3) {
                        throw std::runtime_error("Invalid TextureParameterValues array!");
                    }

                    const auto texParam0Struct = std::dynamic_pointer_cast<s::PropertyStruct>(texParamArray->Values[0]);
                    const auto& texParam0Info = texParam0Struct->Data.get<s::StructProperty>("ParameterInfo");
                    const auto texParam0InfoStruct = std::dynamic_pointer_cast<s::PropertyStruct>(texParam0Info.Value);
                    const auto name0 = texParam0InfoStruct->Data.get<s::NameProperty>("Name").Value.toString();
                    if (name0 != "BaseColorTexture") {
                        throw std::runtime_error("Expected BaseColorTexture!");
                    }
                    const auto& texParam0ValueRef =
                        texParam0Struct->Data.get<s::ObjectProperty>("ParameterValue").Value;
                    const auto tex0Exp = mapAsset.getExportObjectByIdx(texParam0ValueRef.pakValue() - 1);
                    mapTile.texBaseColor = std::make_unique<OGLTexture2D>(*tex0Exp->cast_object<s::UTexture2D>());

                    const auto texParam1Struct = std::dynamic_pointer_cast<s::PropertyStruct>(texParamArray->Values[1]);
                    const auto& texParam1Info = texParam1Struct->Data.get<s::StructProperty>("ParameterInfo");
                    const auto texParam1InfoStruct = std::dynamic_pointer_cast<s::PropertyStruct>(texParam1Info.Value);
                    const auto name1 = texParam1InfoStruct->Data.get<s::NameProperty>("Name").Value.toString();
                    if (name1 != "NormalTexture") {
                        throw std::runtime_error("Expected NormalTexture!");
                    }
                    const auto& texParam1ValueRef =
                        texParam1Struct->Data.get<s::ObjectProperty>("ParameterValue").Value;
                    const auto tex1Exp = mapAsset.getExportObjectByIdx(texParam1ValueRef.pakValue() - 1);
                    mapTile.texNormal = std::make_unique<OGLTexture2D>(*tex1Exp->cast_object<s::UTexture2D>());

                    mapTiles_.push_back(std::move(mapTile));
                }
            }
        } catch (const std::exception& ex) {
            spdlog::warn("Error loading Pak file: {}", ex.what());
        }
    }

    try {
        meshShader_ = std::make_unique<glowl::GLSLProgram>(glowl::GLSLProgram::ShaderSourceList{
            {glowl::GLSLProgram::ShaderType::Vertex, getStringResource("shaders/maptile_mesh.vert")},
            {glowl::GLSLProgram::ShaderType::Fragment, getStringResource("shaders/maptile_mesh.frag")}});

        flatShader_ = std::make_unique<glowl::GLSLProgram>(glowl::GLSLProgram::ShaderSourceList{
            {glowl::GLSLProgram::ShaderType::Vertex, getStringResource("shaders/maptile_flat.vert")},
            {glowl::GLSLProgram::ShaderType::Geometry, getStringResource("shaders/maptile_flat.geom")},
            {glowl::GLSLProgram::ShaderType::Fragment, getStringResource("shaders/maptile_flat.frag")}});
    } catch (glowl::GLSLProgramException& e) {
        spdlog::error(e.what());
    }
}

void Satisfactory3DMap::MapTileRenderer::render(const glm::mat4& projMx, const glm::mat4& viewMx) {
    if (wireframeSetting_->getVal()) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        glDisable(GL_CULL_FACE);
    }

    const std::unique_ptr<glowl::GLSLProgram>& shader = faceNormalsSetting_->getVal() ? flatShader_ : meshShader_;

    shader->use();

    shader->setUniform("projMx", projMx);
    shader->setUniform("viewMx", viewMx);

    for (const auto& tile : mapTiles_) {
        shader->setUniform("modelMx", tile.modelMx);
        shader->setUniform("normalMx", tile.normalMx);

        glActiveTexture(GL_TEXTURE0);
        tile.texBaseColor->bindTexture();
        shader->setUniform("texBaseColor", 0);

        glActiveTexture(GL_TEXTURE1);
        tile.texNormal->bindTexture();
        shader->setUniform("texNormal", 1);

        tile.mesh->draw();
    }

    glUseProgram(0);

    if (wireframeSetting_->getVal()) {
        glEnable(GL_CULL_FACE);
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }
}
