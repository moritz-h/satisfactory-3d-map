#include "MapLODReader.h"

#include <spdlog/spdlog.h>

#include "SatisfactorySave/GameTypes/Arrays/ObjectArray.h"
#include "SatisfactorySave/GameTypes/Arrays/StructArray.h"
#include "SatisfactorySave/GameTypes/Properties/ArrayProperty.h"
#include "SatisfactorySave/GameTypes/Properties/NameProperty.h"
#include "SatisfactorySave/GameTypes/Properties/ObjectProperty.h"
#include "SatisfactorySave/GameTypes/Properties/StructProperty.h"
#include "SatisfactorySave/GameTypes/Structs/PropertyStruct.h"
#include "SatisfactorySave/GameTypes/UE/Engine/GameFramework/Actor.h"

Satisfactory3DMap::MapLODReader::MapLODReader(const std::shared_ptr<SatisfactorySave::PakManager>& pakManager) {
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

                for (std::size_t instCompIdx = 0; instCompIdx < instCompArray->Values.size(); instCompIdx++) {
                    const auto& instCompRef = instCompArray->Values[instCompIdx];
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

                    MapLODMesh LOD_mesh;
                    LOD_mesh.actorLabel = WPHLOD_obj->ActorLabel;
                    LOD_mesh.instanceComponentId = instCompIdx;

                    LOD_mesh.staticMesh = meshObj;

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
                    LOD_mesh.baseColorTexture = tex0Exp->cast_object<s::UTexture2D>();

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
                    LOD_mesh.normalTexture = tex1Exp->cast_object<s::UTexture2D>();

                    meshes_.push_back(std::move(LOD_mesh));
                }
            }
        } catch (const std::exception& ex) {
            spdlog::warn("Error reading Map LOD from Pak file: {}", ex.what());
        }
    }
}
