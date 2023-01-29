#include "ModelManager.h"

#include <glm/gtx/quaternion.hpp>
#include <spdlog/spdlog.h>

#include "GameTypes/Arrays/StructArray.h"
#include "GameTypes/Properties/ArrayProperty.h"
#include "GameTypes/Properties/ObjectProperty.h"
#include "GameTypes/Properties/StructProperty.h"
#include "GameTypes/SaveObjects/SaveActor.h"
#include "GameTypes/Serialization/StaticMesh.h"
#include "GameTypes/Structs/PropertyStruct.h"
#include "GameTypes/Structs/QuatStruct.h"
#include "GameTypes/Structs/RotatorStruct.h"
#include "GameTypes/Structs/VectorStruct.h"
#include "Utils/StringUtils.h"

Satisfactory3DMap::ModelManager::ModelManager(std::shared_ptr<PakManager> pakManager)
    : pakManager_(std::move(pakManager)) {
    meshManager_ = std::make_shared<MeshManager>(pakManager_);

    typedef std::vector<std::pair<std::string, std::vector<std::string>>> modelPathList;

    // First entry in list for fallback
    modelPathList models{
        {"models/cube.glb", {""}},
        {"models/foundation_8x4.glb",
            {"/Game/FactoryGame/Buildable/Building/Foundation/Build_Foundation_8x4_01.Build_Foundation_8x4_01_C"}},
        {"models/foundation_8x2.glb",
            {"/Game/FactoryGame/Buildable/Building/Foundation/Build_Foundation_8x2_01.Build_Foundation_8x2_01_C"}},
        {"models/foundation_8x1.glb",
            {"/Game/FactoryGame/Buildable/Building/Foundation/Build_Foundation_8x1_01.Build_Foundation_8x1_01_C"}},
        {"models/ramp_8x4.glb", {"/Game/FactoryGame/Buildable/Building/Ramp/Build_Ramp_8x4_01.Build_Ramp_8x4_01_C"}},
        {"models/ramp_8x2.glb", {"/Game/FactoryGame/Buildable/Building/Ramp/Build_Ramp_8x2_01.Build_Ramp_8x2_01_C"}},
        {"models/ramp_8x1.glb", {"/Game/FactoryGame/Buildable/Building/Ramp/Build_Ramp_8x1_01.Build_Ramp_8x1_01_C"}},
        {"models/doubleramp_8x4.glb",
            {"/Game/FactoryGame/Buildable/Building/Ramp/Build_Ramp_8x8x8.Build_Ramp_8x8x8_C"}},
        {"models/doubleramp_8x2.glb",
            {"/Game/FactoryGame/Buildable/Building/Ramp/Build_RampDouble.Build_RampDouble_C"}},
        {"models/doubleramp_8x1.glb",
            {"/Game/FactoryGame/Buildable/Building/Ramp/Build_RampDouble_8x1.Build_RampDouble_8x1_C"}},
        {"models/wall.glb", {"/Game/FactoryGame/Buildable/Building/Wall/Build_Wall_8x4_01.Build_Wall_8x4_01_C"}},
        {"models/powerpole.glb",
            {"/Game/FactoryGame/Buildable/Factory/PowerPoleMk1/Build_PowerPoleMk1.Build_PowerPoleMk1_C",
                "/Game/FactoryGame/Buildable/Factory/PowerPoleMk2/Build_PowerPoleMk2.Build_PowerPoleMk2_C",
                "/Game/FactoryGame/Buildable/Factory/PowerPoleMk3/Build_PowerPoleMk3.Build_PowerPoleMk3_C"}},
        {"models/splitter.glb",
            {"/Game/FactoryGame/Buildable/Factory/CA_Merger/"
             "Build_ConveyorAttachmentMerger.Build_ConveyorAttachmentMerger_C",
                "/Game/FactoryGame/Buildable/Factory/CA_Splitter/"
                "Build_ConveyorAttachmentSplitter.Build_ConveyorAttachmentSplitter_C",
                "/Game/FactoryGame/Buildable/Factory/CA_SplitterSmart/"
                "Build_ConveyorAttachmentSplitterSmart.Build_ConveyorAttachmentSplitterSmart_C",
                "/Game/FactoryGame/Buildable/Factory/CA_SplitterProgrammable/"
                "Build_ConveyorAttachmentSplitterProgrammable.Build_ConveyorAttachmentSplitterProgrammable_C"}},
    };

    modelPathList splineModels{
        {"models/spline_mesh/conveyor_belt.glb",
            {
                "/Game/FactoryGame/Buildable/Factory/ConveyorBeltMk1/Build_ConveyorBeltMk1.Build_ConveyorBeltMk1_C",
                "/Game/FactoryGame/Buildable/Factory/ConveyorBeltMk2/Build_ConveyorBeltMk2.Build_ConveyorBeltMk2_C",
                "/Game/FactoryGame/Buildable/Factory/ConveyorBeltMk3/Build_ConveyorBeltMk3.Build_ConveyorBeltMk3_C",
                "/Game/FactoryGame/Buildable/Factory/ConveyorBeltMk4/Build_ConveyorBeltMk4.Build_ConveyorBeltMk4_C",
                "/Game/FactoryGame/Buildable/Factory/ConveyorBeltMk5/Build_ConveyorBeltMk5.Build_ConveyorBeltMk5_C",
            }},
        {"models/spline_mesh/pipe.glb",
            {
                "/Game/FactoryGame/Buildable/Factory/Pipeline/Build_Pipeline.Build_Pipeline_C",
                "/Game/FactoryGame/Buildable/Factory/PipelineMk2/Build_PipelineMK2.Build_PipelineMK2_C",
                "/Game/FactoryGame/Buildable/Factory/PipeHyper/Build_PipeHyper.Build_PipeHyper_C",
            }},
        {"models/spline_mesh/track.glb",
            {
                "/Game/FactoryGame/Buildable/Factory/Train/Track/Build_RailroadTrack.Build_RailroadTrack_C",
                "/Game/FactoryGame/Buildable/Factory/Train/Track/"
                "Build_RailroadTrackIntegrated.Build_RailroadTrackIntegrated_C",
            }},
    };

    for (const auto& entry : models) {
        models_.emplace_back(std::make_unique<GltfModel>(entry.first));
        modelSavePaths_.emplace_back(entry.second);
    }
    for (const auto& entry : splineModels) {
        splineModels_.emplace_back(std::make_unique<GltfModel>(entry.first));
        splineModelSavePaths_.emplace_back(entry.second);
    }
}

std::pair<Satisfactory3DMap::ModelManager::ModelType, int32_t> Satisfactory3DMap::ModelManager::classifyActor(
    const SaveActor& a) {

    const auto& className = a.className();

    if (pakManager_ != nullptr) {
        const auto& pakModel = findPakModel(className);
        if (pakModel.has_value()) {
            return std::make_pair(ModelType::PakStaticMesh, pakModel.value());
        }
    }

    for (int32_t i = 0; i < static_cast<int32_t>(splineModels_.size()); i++) {
        for (const auto& name : splineModelSavePaths_[i]) {
            if (className == name) {
                return std::make_pair(ModelType::SplineModel, i);
            }
        }
    }

    // model idx 0 is fallback model, start search with i = 1.
    for (int32_t i = 1; i < static_cast<int32_t>(models_.size()); i++) {
        for (const auto& name : modelSavePaths_[i]) {
            if (className == name) {
                return std::make_pair(ModelType::Model, i);
            }
        }
    }

    // Ignore Script Actors
    if (startsWith(className, "/Script/")) {
        return std::make_pair(ModelType::None, -1);
    }

    return std::make_pair(ModelType::Model, 0);
}

std::optional<int32_t> Satisfactory3DMap::ModelManager::findPakModel(const std::string& className) {
    if (!startsWith(className, "/Game/FactoryGame/Buildable/")) {
        return std::nullopt;
    }
    // TODO for now only buildings
    if (!startsWith(className, "/Game/FactoryGame/Buildable/Building/")) {
        return std::nullopt;
    }

    if (classNameToPakModelMap_.count(className) > 0) {
        return static_cast<int32_t>(classNameToPakModelMap_.at(className));
    }
    if (classNamesNotInPak_.count(className) > 0) {
        return std::nullopt;
    }

    std::size_t idx = 0;
    try {
        idx = loadAsset(className);
    } catch (const std::exception& ex) {
        classNamesNotInPak_.insert(className);
        spdlog::error(ex.what());
        return std::nullopt;
    }

    classNameToPakModelMap_[className] = idx;
    return static_cast<int32_t>(idx);
}

std::size_t Satisfactory3DMap::ModelManager::loadAsset(const std::string& className) {
    std::string assetName = PakManager::classNameToAssetPath(className);

    if (!pakManager_->containsAssetFilename(assetName)) {
        throw std::runtime_error("Asset missing: " + assetName);
    }

    auto asset = pakManager_->readAsset(assetName);

    const auto defaultObjectName = "Default__" + PakManager::classNameToObjectName(className);
    if (asset.hasObjectExportEntry(defaultObjectName)) {
        const auto defaultObjectExportEntry = asset.getObjectExportEntry(defaultObjectName);

        asset.seek(defaultObjectExportEntry.SerialOffset);
        Properties defaultObjectProperties;
        asset << defaultObjectProperties;

        try {
            const auto& instanceDataCDO = defaultObjectProperties.get<ObjectProperty>("mInstanceDataCDO");
            if (instanceDataCDO.value().pakValue() < 1) {
                spdlog::error("Instance data pak index < 1!");
                throw std::runtime_error("Instance data pak index < 1!");
            }
            const auto& instanceDataExportEntry = asset.exportMap()[instanceDataCDO.value().pakValue() - 1];

            asset.seek(instanceDataExportEntry.SerialOffset);
            Properties instanceDataProperties;
            asset << instanceDataProperties;

            const auto* instances =
                dynamic_cast<StructArray*>(instanceDataProperties.get<ArrayProperty>("Instances").array().get());
            if (instances == nullptr || instances->array().empty()) {
                throw std::runtime_error("Instances not found or empty!");
            }

            std::vector<std::shared_ptr<StaticMesh>> meshes;
            std::vector<glm::mat4> transforms;
            for (const auto& item : instances->array()) {
                auto [mesh, transform] = getStaticMeshTransformFromStruct(asset, item);
                meshes.push_back(std::move(mesh));
                transforms.push_back(transform);
            }

            // TODO support multiple meshes, for not just use the first one
            const auto num = pakModels_.size();
            pakModels_.emplace_back(std::make_unique<StaticMeshVAO>(*meshes[0]));
            pakTransformations_.emplace_back(transforms[0]);
            return num;
        } catch (...) {}
    }

    int buildingMeshProxyExportId = -1;
    for (int i = 0; i < asset.exportMap().size(); i++) {
        const auto& exp = asset.exportMap()[i];
        if (exp.ObjectName == "BuildingMeshProxy") {
            if (buildingMeshProxyExportId >= 0) {
                throw std::runtime_error("Asset has more than one BuildingMeshProxy: " + assetName);
            }
            buildingMeshProxyExportId = i;
        }
    }
    if (buildingMeshProxyExportId < 0) {
        throw std::runtime_error("Cannot read BuildingMeshProxy: " + assetName);
    }
    const auto& buildingMeshProxyExport = asset.exportMap()[buildingMeshProxyExportId];

    asset.seek(buildingMeshProxyExport.SerialOffset);
    Properties properties;
    asset << properties;

    ObjectReference objectReference;
    try {
        objectReference = properties.get<ObjectProperty>("StaticMesh").value();
    } catch ([[maybe_unused]] const std::exception& e) {
        throw std::runtime_error("Asset does not have StaticMesh property: " + assetName);
    }
    auto mesh = readStaticMeshFromReference(asset, objectReference);

    glm::mat4 translationMx(1.0f);
    try {
        const auto& locationStructProp = properties.get<StructProperty>("RelativeLocation").value();
        const auto* locationStruct = dynamic_cast<const VectorStruct*>(locationStructProp.get());
        if (locationStruct != nullptr) {
            translationMx = glm::translate(glm::mat4(1.0f), locationStruct->value());
        }
    } catch ([[maybe_unused]] const std::exception& e) {}

    glm::mat4 rotationMx(1.0f);
    try {
        const auto& rotationStructProp = properties.get<StructProperty>("RelativeRotation").value();
        const auto* rotationStruct = dynamic_cast<const RotatorStruct*>(rotationStructProp.get());
        if (rotationStruct != nullptr) {
            rotationMx = glm::toMat4(rotationStruct->quaternion());
        }
    } catch ([[maybe_unused]] const std::exception& e) {}

    glm::mat4 modelMx = translationMx * rotationMx;

    const auto num = pakModels_.size();
    pakModels_.emplace_back(std::make_unique<StaticMeshVAO>(*mesh));
    pakTransformations_.emplace_back(modelMx);
    return num;
}

std::shared_ptr<Satisfactory3DMap::StaticMesh> Satisfactory3DMap::ModelManager::readStaticMeshFromReference(
    AssetFile& asset, const Satisfactory3DMap::ObjectReference& objectReference) {

    if (objectReference.pakValue() >= 0) {
        throw std::runtime_error("StaticMeshReference >= 0 not implemented!");
    }

    const auto& StaticMeshImport = asset.importMap()[-objectReference.pakValue() - 1];

    if (StaticMeshImport.OuterIndex >= 0) {
        throw std::runtime_error("StaticMeshImport.OuterIndex >= 0 not implemented!");
    }

    std::string StaticMeshAssetName = asset.importMap()[-StaticMeshImport.OuterIndex - 1].ObjectName.toString();
    return meshManager_->loadMesh(StaticMeshAssetName + "." + StaticMeshImport.ObjectName.toString());
}

std::tuple<std::shared_ptr<Satisfactory3DMap::StaticMesh>, glm::mat4>
Satisfactory3DMap::ModelManager::getStaticMeshTransformFromStruct(AssetFile& asset,
    const std::unique_ptr<Struct>& instanceDataStruct) {

    const auto* instanceData = dynamic_cast<PropertyStruct*>(instanceDataStruct.get());
    if (instanceData == nullptr) {
        throw std::runtime_error("Unexpected type!");
    }

    const auto& staticMeshRef = instanceData->properties().get<ObjectProperty>("StaticMesh").value();
    auto mesh = readStaticMeshFromReference(asset, staticMeshRef);

    glm::mat4 modelMx = glm::mat4(1.0f);
    try {
        const auto& relativeTransform = instanceData->properties().get<StructProperty>("RelativeTransform");
        const auto* relativeTransformStruct = dynamic_cast<PropertyStruct*>(relativeTransform.value().get());
        if (relativeTransformStruct == nullptr) {
            throw std::runtime_error("Bad struct type!");
        }

        const auto* Rotation = dynamic_cast<const QuatStruct*>(
            relativeTransformStruct->properties().get<StructProperty>("Rotation").value().get());
        const auto* Translation = dynamic_cast<const VectorStruct*>(
            relativeTransformStruct->properties().get<StructProperty>("Translation").value().get());
        const auto* Scale3D = dynamic_cast<const VectorStruct*>(
            relativeTransformStruct->properties().get<StructProperty>("Scale3D").value().get());
        if (Rotation == nullptr || Translation == nullptr || Scale3D == nullptr) {
            throw std::runtime_error("Bad struct types!");
        }

        const glm::mat4 rotationMx = glm::toMat4(glm::quat(Rotation->w(), Rotation->x(), Rotation->y(), Rotation->z()));
        const glm::mat4 translationMx = glm::translate(glm::mat4(1.0f), Translation->value());
        const glm::mat4 scaleMx = glm::scale(glm::mat4(1.0f), Scale3D->value());

        modelMx = translationMx * rotationMx * scaleMx;
    } catch (...) {}

    return std::make_tuple(std::move(mesh), modelMx);
}
