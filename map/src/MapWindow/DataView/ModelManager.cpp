#include "ModelManager.h"

#include <glm/gtc/quaternion.hpp>
#include <spdlog/spdlog.h>

#include "SatisfactorySave/GameTypes/Arrays/StructArray.h"
#include "SatisfactorySave/GameTypes/Properties/ArrayProperty.h"
#include "SatisfactorySave/GameTypes/Properties/ObjectProperty.h"
#include "SatisfactorySave/GameTypes/Properties/StructProperty.h"
#include "SatisfactorySave/GameTypes/Structs/PropertyStruct.h"
#include "SatisfactorySave/GameTypes/Structs/QuatStruct.h"
#include "SatisfactorySave/GameTypes/Structs/RotatorStruct.h"
#include "SatisfactorySave/GameTypes/Structs/VectorStruct.h"
#include "SatisfactorySave/GameTypes/UE/Engine/Engine/StaticMesh.h"
#include "SatisfactorySave/Utils/StringUtils.h"

#include "Utils/GLMUtil.h"

Satisfactory3DMap::ModelManager::ModelManager(std::shared_ptr<s::PakManager> pakManager)
    : pakManager_(std::move(pakManager)) {
    meshManager_ = std::make_shared<MeshManager>(pakManager_);

    typedef std::vector<std::pair<std::string, std::vector<std::string>>> modelPathList;

    // First entry in list for fallback
    modelPathList models{
        {"models/cube.glb", {""}},
        {
            "models/foundation_8x4.glb",
            {
                "/Game/FactoryGame/Buildable/Building/Foundation/Build_Foundation_8x4_01.Build_Foundation_8x4_01_C",
                "/Game/FactoryGame/Buildable/Building/Foundation/ConcreteSet/Build_Foundation_Concrete_8x4.Build_Foundation_Concrete_8x4_C",
                "/Game/FactoryGame/Buildable/Building/Foundation/GripMetal/Build_Foundation_Metal_8x4.Build_Foundation_Metal_8x4_C",
                "/Game/FactoryGame/Buildable/Building/Foundation/PolishedConcreteSet/Build_Foundation_ConcretePolished_8x4.Build_Foundation_ConcretePolished_8x4_C",
                "/Game/FactoryGame/Buildable/Building/Foundation/AsphaltSet/Build_Foundation_Asphalt_8x4.Build_Foundation_Asphalt_8x4_C",
            },
        },
        {
            "models/foundation_8x2.glb",
            {
                "/Game/FactoryGame/Buildable/Building/Foundation/Build_Foundation_8x2_01.Build_Foundation_8x2_01_C",
                "/Game/FactoryGame/Buildable/Building/Foundation/ConcreteSet/Build_Foundation_Concrete_8x2.Build_Foundation_Concrete_8x2_C",
                "/Game/FactoryGame/Buildable/Building/Foundation/GripMetal/Build_Foundation_Metal_8x2.Build_Foundation_Metal_8x2_C",
                "/Game/FactoryGame/Buildable/Building/Foundation/PolishedConcreteSet/Build_Foundation_ConcretePolished_8x2_2.Build_Foundation_ConcretePolished_8x2_2_C",
                "/Game/FactoryGame/Buildable/Building/Foundation/AsphaltSet/Build_Foundation_Asphalt_8x2.Build_Foundation_Asphalt_8x2_C",
            },
        },
        {
            "models/foundation_8x1.glb",
            {
                "/Game/FactoryGame/Buildable/Building/Foundation/Build_Foundation_8x1_01.Build_Foundation_8x1_01_C",
                "/Game/FactoryGame/Buildable/Building/Foundation/ConcreteSet/Build_Foundation_Concrete_8x1.Build_Foundation_Concrete_8x1_C",
                "/Game/FactoryGame/Buildable/Building/Foundation/GripMetal/Build_Foundation_Metal_8x1.Build_Foundation_Metal_8x1_C",
                "/Game/FactoryGame/Buildable/Building/Foundation/PolishedConcreteSet/Build_Foundation_ConcretePolished_8x1.Build_Foundation_ConcretePolished_8x1_C",
                "/Game/FactoryGame/Buildable/Building/Foundation/AsphaltSet/Build_Foundation_Asphalt_8x1.Build_Foundation_Asphalt_8x1_C",
            },
        },
        {
            "models/ramp_8x4.glb",
            {
                "/Game/FactoryGame/Buildable/Building/Ramp/Build_Ramp_8x4_01.Build_Ramp_8x4_01_C",
                "/Game/FactoryGame/Buildable/Building/Foundation/ConcreteSet/Build_Ramp_Concrete_8x4.Build_Ramp_Concrete_8x4_C",
                "/Game/FactoryGame/Buildable/Building/Foundation/GripMetal/Build_Ramp_Metal_8x4.Build_Ramp_Metal_8x4_C",
                "/Game/FactoryGame/Buildable/Building/Foundation/PolishedConcreteSet/Build_Ramp_Polished_8x4.Build_Ramp_Polished_8x4_C",
                "/Game/FactoryGame/Buildable/Building/Foundation/AsphaltSet/Build_Ramp_Asphalt_8x4.Build_Ramp_Asphalt_8x4_C",
            },
        },
        {
            "models/ramp_8x2.glb",
            {
                "/Game/FactoryGame/Buildable/Building/Ramp/Build_Ramp_8x2_01.Build_Ramp_8x2_01_C",
                "/Game/FactoryGame/Buildable/Building/Foundation/ConcreteSet/Build_Ramp_Concrete_8x2.Build_Ramp_Concrete_8x2_C",
                "/Game/FactoryGame/Buildable/Building/Foundation/GripMetal/Build_Ramp_Metal_8x2.Build_Ramp_Metal_8x2_C",
                "/Game/FactoryGame/Buildable/Building/Foundation/PolishedConcreteSet/Build_Ramp_Polished_8x2.Build_Ramp_Polished_8x2_C",
                "/Game/FactoryGame/Buildable/Building/Foundation/AsphaltSet/Build_Ramp_Asphalt_8x2.Build_Ramp_Asphalt_8x2_C",
            },
        },
        {
            "models/ramp_8x1.glb",
            {
                "/Game/FactoryGame/Buildable/Building/Ramp/Build_Ramp_8x1_01.Build_Ramp_8x1_01_C",
                "/Game/FactoryGame/Buildable/Building/Foundation/ConcreteSet/Build_Ramp_Concrete_8x1.Build_Ramp_Concrete_8x1_C",
                "/Game/FactoryGame/Buildable/Building/Foundation/GripMetal/Build_Ramp_Metal_8x1.Build_Ramp_Metal_8x1_C",
                "/Game/FactoryGame/Buildable/Building/Foundation/PolishedConcreteSet/Build_Ramp_Polished_8x1.Build_Ramp_Polished_8x1_C",
                "/Game/FactoryGame/Buildable/Building/Foundation/AsphaltSet/Build_Ramp_Asphalt_8x1.Build_Ramp_Asphalt_8x1_C",
            },
        },
        {
            "models/doubleramp_8x4.glb",
            {
                "/Game/FactoryGame/Buildable/Building/Ramp/Build_Ramp_8x8x8.Build_Ramp_8x8x8_C",
                "/Game/FactoryGame/Buildable/Building/Foundation/ConcreteSet/Build_RampDouble_Concrete_8x4.Build_RampDouble_Concrete_8x4_C",
                "/Game/FactoryGame/Buildable/Building/Foundation/GripMetal/Build_RampDouble_Metal_8x4.Build_RampDouble_Metal_8x4_C",
                "/Game/FactoryGame/Buildable/Building/Foundation/PolishedConcreteSet/Build_RampDouble_Polished_8x4.Build_RampDouble_Polished_8x4_C",
                "/Game/FactoryGame/Buildable/Building/Foundation/AsphaltSet/Build_RampDouble_Asphalt_8x4.Build_RampDouble_Asphalt_8x4_C",
            },
        },
        {
            "models/doubleramp_8x2.glb",
            {
                "/Game/FactoryGame/Buildable/Building/Ramp/Build_RampDouble.Build_RampDouble_C",
                "/Game/FactoryGame/Buildable/Building/Foundation/ConcreteSet/Build_RampDouble_Concrete_8x2.Build_RampDouble_Concrete_8x2_C",
                "/Game/FactoryGame/Buildable/Building/Foundation/GripMetal/Build_RampDouble_Metal_8x2.Build_RampDouble_Metal_8x2_C",
                "/Game/FactoryGame/Buildable/Building/Foundation/PolishedConcreteSet/Build_RampDouble_Polished_8x2.Build_RampDouble_Polished_8x2_C",
                "/Game/FactoryGame/Buildable/Building/Foundation/AsphaltSet/Build_RampDouble_Asphalt_8x2.Build_RampDouble_Asphalt_8x2_C",
            },
        },
        {
            "models/doubleramp_8x1.glb",
            {
                "/Game/FactoryGame/Buildable/Building/Ramp/Build_RampDouble_8x1.Build_RampDouble_8x1_C",
                "/Game/FactoryGame/Buildable/Building/Foundation/ConcreteSet/Build_RampDouble_Concrete_8x1.Build_RampDouble_Concrete_8x1_C",
                "/Game/FactoryGame/Buildable/Building/Foundation/GripMetal/Build_RampDouble_Metal_8x1.Build_RampDouble_Metal_8x1_C",
                "/Game/FactoryGame/Buildable/Building/Foundation/PolishedConcreteSet/Build_RampDouble_Polished_8x1.Build_RampDouble_Polished_8x1_C",
                "/Game/FactoryGame/Buildable/Building/Foundation/AsphaltSet/Build_RampDouble_Asphalt_8x1.Build_RampDouble_Asphalt_8x1_C",
            },
        },
        {
            "models/wall.glb",
            {
                "/Game/FactoryGame/Buildable/Building/Wall/Build_Wall_8x4_01.Build_Wall_8x4_01_C",
            },
        },
        {
            "models/powerpole.glb",
            {
                "/Game/FactoryGame/Buildable/Factory/PowerPoleMk1/Build_PowerPoleMk1.Build_PowerPoleMk1_C",
                "/Game/FactoryGame/Buildable/Factory/PowerPoleMk2/Build_PowerPoleMk2.Build_PowerPoleMk2_C",
                "/Game/FactoryGame/Buildable/Factory/PowerPoleMk3/Build_PowerPoleMk3.Build_PowerPoleMk3_C",
            },
        },
        {
            "models/splitter.glb",
            {
                "/Game/FactoryGame/Buildable/Factory/CA_Merger/"
                "Build_ConveyorAttachmentMerger.Build_ConveyorAttachmentMerger_C",
                "/Game/FactoryGame/Buildable/Factory/CA_Splitter/"
                "Build_ConveyorAttachmentSplitter.Build_ConveyorAttachmentSplitter_C",
                "/Game/FactoryGame/Buildable/Factory/CA_SplitterSmart/"
                "Build_ConveyorAttachmentSplitterSmart.Build_ConveyorAttachmentSplitterSmart_C",
                "/Game/FactoryGame/Buildable/Factory/CA_SplitterProgrammable/"
                "Build_ConveyorAttachmentSplitterProgrammable.Build_ConveyorAttachmentSplitterProgrammable_C",
            },
        },
    };

    modelPathList splineModels{
        {
            "models/spline_mesh/conveyor_belt.glb",
            {
                "/Game/FactoryGame/Buildable/Factory/ConveyorBeltMk1/Build_ConveyorBeltMk1.Build_ConveyorBeltMk1_C",
                "/Game/FactoryGame/Buildable/Factory/ConveyorBeltMk2/Build_ConveyorBeltMk2.Build_ConveyorBeltMk2_C",
                "/Game/FactoryGame/Buildable/Factory/ConveyorBeltMk3/Build_ConveyorBeltMk3.Build_ConveyorBeltMk3_C",
                "/Game/FactoryGame/Buildable/Factory/ConveyorBeltMk4/Build_ConveyorBeltMk4.Build_ConveyorBeltMk4_C",
                "/Game/FactoryGame/Buildable/Factory/ConveyorBeltMk5/Build_ConveyorBeltMk5.Build_ConveyorBeltMk5_C",
                "/Game/FactoryGame/Buildable/Factory/ConveyorBeltMk6/Build_ConveyorBeltMk6.Build_ConveyorBeltMk6_C",
            },
        },
        {
            "models/spline_mesh/pipe.glb",
            {
                "/Game/FactoryGame/Buildable/Factory/Pipeline/Build_Pipeline.Build_Pipeline_C",
                "/Game/FactoryGame/Buildable/Factory/PipelineMk2/Build_PipelineMK2.Build_PipelineMK2_C",
                "/Game/FactoryGame/Buildable/Factory/PipeHyper/Build_PipeHyper.Build_PipeHyper_C",
            },
        },
        {
            "models/spline_mesh/track.glb",
            {
                "/Game/FactoryGame/Buildable/Factory/Train/Track/Build_RailroadTrack.Build_RailroadTrack_C",
                "/Game/FactoryGame/Buildable/Factory/Train/Track/"
                "Build_RailroadTrackIntegrated.Build_RailroadTrackIntegrated_C",
            },
        },
    };

    std::vector<std::string> powerLineClasses{
        "/Game/FactoryGame/Buildable/Factory/PowerLine/Build_PowerLine.Build_PowerLine_C",
        "/Game/FactoryGame/Events/Christmas/Buildings/PowerLineLights/Build_XmassLightsLine.Build_XmassLightsLine_C",
    };

    for (const auto& entry : models) {
        models_.emplace_back(std::make_unique<GltfModel>(entry.first));
        modelSavePaths_.emplace_back(entry.second);
    }
    for (const auto& entry : splineModels) {
        splineModels_.emplace_back(std::make_unique<GltfModel>(entry.first));
        splineModelSavePaths_.emplace_back(entry.second);
    }
    powerLineSavePaths_ = std::unordered_set<std::string>(powerLineClasses.begin(), powerLineClasses.end());
}

std::pair<Satisfactory3DMap::ModelManager::ModelType, int32_t> Satisfactory3DMap::ModelManager::classifyActor(
    const std::string& className) {

    // Ignore Script Actors
    if (className.starts_with("/Script/")) {
        return std::make_pair(ModelType::None, -1);
    }

    // Models with special shaders (splines, power lines)
    for (int32_t i = 0; i < static_cast<int32_t>(splineModels_.size()); i++) {
        for (const auto& name : splineModelSavePaths_[i]) {
            if (className == name) {
                return std::make_pair(ModelType::SplineModel, i);
            }
        }
    }

    if (powerLineSavePaths_.contains(className)) {
        return std::make_pair(ModelType::PowerLine, -1);
    }

    // Search static meshes in pak files
    if (pakManager_ != nullptr) {
        const auto& pakModel = findPakModel(className);
        if (pakModel.has_value()) {
            return std::make_pair(ModelType::PakStaticMesh, pakModel.value());
        }
    }

    // Search bundled GLTF models
    // model idx 0 is fallback model, start search with i = 1.
    for (int32_t i = 1; i < static_cast<int32_t>(models_.size()); i++) {
        for (const auto& name : modelSavePaths_[i]) {
            if (className == name) {
                return std::make_pair(ModelType::Model, i);
            }
        }
    }

    // Use default box model
    return std::make_pair(ModelType::Model, 0);
}

std::optional<int32_t> Satisfactory3DMap::ModelManager::findPakModel(const std::string& className) {
    if (!className.starts_with("/Game/FactoryGame/Buildable/")) {
        return std::nullopt;
    }
    // TODO for now only buildings and factories
    if (!className.starts_with("/Game/FactoryGame/Buildable/Building/") &&
        !className.starts_with("/Game/FactoryGame/Buildable/Factory/")) {
        return std::nullopt;
    }

    if (classNameToPakModelMap_.contains(className)) {
        return static_cast<int32_t>(classNameToPakModelMap_.at(className));
    }
    if (classNamesNotInPak_.contains(className)) {
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
    std::string assetName = s::PakManager::classNameToAssetPath(className);

    if (!pakManager_->containsAssetFilename(assetName)) {
        throw std::runtime_error("Asset missing: " + assetName);
    }

    auto asset = pakManager_->readAsset(assetName);

    const auto defaultObjectName = "Default__" + s::PakManager::classNameToObjectName(className);
    if (!asset.hasExportMapEntry(defaultObjectName)) {
        throw std::runtime_error("Default object missing!");
    }
    const auto defaultObject = asset.getExportObjectByName(defaultObjectName);
    if (defaultObject->Object == nullptr) {
        throw std::runtime_error("Default object is null!");
    }

    std::optional<MeshModel> model;
    if (!model.has_value()) {
        model = tryReadHardcodedMeshList(className);
    }
    if (!model.has_value()) {
        model = tryReadInstanceDataCDO(asset, *defaultObject);
    }
    if (!model.has_value()) {
        model = tryReadMeshComponentProxy(asset, *defaultObject);
    }

    if (model.has_value()) {
        const auto num = pakModels_.size();
        pakModels_.push_back(std::move(model.value()));
        return num;
    }

    throw std::runtime_error("No mesh found: " + className);
}

std::optional<Satisfactory3DMap::ModelManager::MeshModel> Satisfactory3DMap::ModelManager::tryReadHardcodedMeshList(
    const std::string& className) {
    static std::unordered_map<std::string, std::vector<std::pair<std::string, uint64_t>>> meshList{
        {
            "/Game/FactoryGame/Buildable/Factory/ConstructorMk1/Build_ConstructorMk1.Build_ConstructorMk1_C",
            {
                {"/Game/FactoryGame/Buildable/Factory/ConstructorMk1/Mesh/ConstructorMk1_static",
                    17531475687992568172u},
                {"/Game/FactoryGame/Buildable/Factory/ConstructorMk1/Mesh/SM_VAT_Constructor_MK1",
                    8788781691149688441u},
            },
        },
        {
            "/Game/FactoryGame/Buildable/Factory/AssemblerMk1/Build_AssemblerMk1.Build_AssemblerMk1_C",
            {
                {"/Game/FactoryGame/Buildable/Factory/AssemblerMk1/Mesh/AssemblerMk1_static", 2876686963785518280u},
                {"/Game/FactoryGame/Buildable/Factory/AssemblerMk1/Mesh/SM_Assembler_VAT", 1601938478383505850u},
            },
        },
        {
            "/Game/FactoryGame/Buildable/Factory/ManufacturerMk1/Build_ManufacturerMk1.Build_ManufacturerMk1_C",
            {
                {"/Game/FactoryGame/Buildable/Factory/ManufacturerMk1/Mesh/SM_Manufacturer", 16596215462250294140u},
                {"/Game/FactoryGame/Buildable/Factory/ManufacturerMk1/Mesh/SM_VAT_Manufacturer", 6258458244933010946u},
            },
        },
        {
            "/Game/FactoryGame/Buildable/Factory/Packager/Build_Packager.Build_Packager_C",
            {
                {"/Game/FactoryGame/Buildable/Factory/Packager/Mesh/SM_Packager_01", 3412523861746888800u},

            },
        },
        {
            "/Game/FactoryGame/Buildable/Factory/OilRefinery/Build_OilRefinery.Build_OilRefinery_C",
            {
                {"/Game/FactoryGame/Buildable/Factory/OilRefinery/Mesh/SM_OilRefinery_01", 3116309774917793760u},
                {"/Game/FactoryGame/Buildable/Factory/OilRefinery/Mesh/SM_Refinery_VAT_01", 15001394164382916637u},
            },
        },
        {
            "/Game/FactoryGame/Buildable/Factory/Blender/Build_Blender.Build_Blender_C",
            {
                {"/Game/FactoryGame/Buildable/Factory/Blender/Mesh/SM_Blender_01", 1455697199552998628u},
                {"/Game/FactoryGame/Buildable/Factory/Blender/Mesh/SM_Blender_Mixer_01", 6028939070185310187u},
                {"/Game/FactoryGame/Buildable/Factory/Blender/Mesh/SM_Blender_VAT", 10110221954603884013u},
            },
        },
        {
            "/Game/FactoryGame/Buildable/Factory/HadronCollider/Build_HadronCollider.Build_HadronCollider_C",
            {
                {"/Game/FactoryGame/Buildable/Factory/HadronCollider/Mesh/SM_HadronCollider_01", 685014826575293139u},
                {"/Game/FactoryGame/Buildable/Factory/HadronCollider/Mesh/SM_HC_Spool_01", 17213070446062009596u},
                {"/Game/FactoryGame/Buildable/Factory/HadronCollider/Mesh/SM_HC_Spool_02", 14653539250387620066u},
            },
        },
        {
            "/Game/FactoryGame/Buildable/Factory/QuantumEncoder/Build_QuantumEncoder.Build_QuantumEncoder_C",
            {
                {"/Game/FactoryGame/Buildable/Factory/QuantumEncoder/Mesh/SM_QuantumEncoder_01", 14287920513212878460u},

            },
        },
        {
            "/Game/FactoryGame/Buildable/Factory/Converter/Build_Converter.Build_Converter_C",
            {
                {"/Game/FactoryGame/Buildable/Factory/Converter/Mesh/SM_Converter_01", 5689439705689270845u},
                {"/Game/FactoryGame/Buildable/Factory/Converter/Mesh/SM_VAT_Converter", 10505819839463466523u},
            },
        },
    };

    auto it = meshList.find(className);
    if (it == meshList.end()) {
        return std::nullopt;
    }

    MeshModel model;
    model.reserve(it->second.size());
    for (const auto& [packageName, publicExportHash] : it->second) {
        // Try catch for logging, errors on hardcoded paths should be visible in log.
        try {
            model.push_back({meshManager_->loadMesh(packageName, publicExportHash), glm::mat4(1.0f)});
        } catch (const std::exception& ex) {
            spdlog::error("Error reading hard-coded mesh: {}", ex.what());
            throw;
        }
    }
    return model;
}

std::optional<Satisfactory3DMap::ModelManager::MeshModel> Satisfactory3DMap::ModelManager::tryReadInstanceDataCDO(
    s::AssetFile& asset, const s::AssetExport& defaultObject) {
    try {
        const auto& instanceDataCDO = defaultObject.Object->Properties.get<s::ObjectProperty>("mInstanceDataCDO");
        if (instanceDataCDO.Value.pakValue() < 1) {
            spdlog::error("Instance data pak index < 1!");
            return std::nullopt;
        }

        const auto& instanceData = asset.getExportObjectByIdx(instanceDataCDO.Value.pakValue() - 1);
        const auto& instances =
            instanceData->Object->Properties.get<s::ArrayProperty>("Instances").get<s::StructArray>();
        if (instances.Values.empty()) {
            throw std::runtime_error("Instances are empty!");
        }

        MeshModel model;
        for (const auto& item : instances.Values) {
            model.push_back(getStaticMeshTransformFromStruct(asset, item));
        }

        return model;
    } catch (const std::exception& ex) {
        return std::nullopt;
    }
}

std::optional<Satisfactory3DMap::ModelManager::MeshModel> Satisfactory3DMap::ModelManager::tryReadMeshComponentProxy(
    s::AssetFile& asset, const s::AssetExport& defaultObject) {
    try {
        const auto& proxyRef = defaultObject.Object->Properties.get<s::ObjectProperty>("mMeshComponentProxy");
        if (proxyRef.Value.pakValue() == 0) {
            return std::nullopt;
        }
        if (proxyRef.Value.pakValue() < 0) {
            spdlog::error("mMeshComponentProxy pakValue reference <0 not implemented!");
            return std::nullopt;
        }

        auto buildingMeshProxy = asset.getExportObjectByIdx(proxyRef.Value.pakValue() - 1);
        const auto& properties = buildingMeshProxy->Object->Properties;

        s::FObjectReferenceDisc objectReference = properties.get<s::ObjectProperty>("StaticMesh").Value;
        auto mesh = readStaticMeshFromReference(asset, objectReference);

        glm::mat4 translationMx(1.0f);
        try {
            const auto& location = properties.get<s::StructProperty>("RelativeLocation").get<s::VectorStruct>().Data;
            translationMx = glm::translate(glm::mat4(1.0f), glmCast(location));
        } catch ([[maybe_unused]] const std::exception& e) {}

        glm::mat4 rotationMx(1.0f);
        try {
            const auto& rotation = properties.get<s::StructProperty>("RelativeRotation").get<s::RotatorStruct>().Data;
            rotationMx = glm::mat4_cast(glmCast(rotation.Quaternion()));
        } catch ([[maybe_unused]] const std::exception& e) {}

        glm::mat4 modelMx = translationMx * rotationMx;

        return std::make_optional<MeshModel>({{mesh, modelMx}});
    } catch (const std::exception& ex) {
        return std::nullopt;
    }
}

std::shared_ptr<glowl::Mesh> Satisfactory3DMap::ModelManager::readStaticMeshFromReference(s::AssetFile& asset,
    const s::FObjectReferenceDisc& objectReference) {

    if (objectReference.pakValue() >= 0) {
        throw std::runtime_error("StaticMeshReference >= 0 not implemented!");
    }

    const auto& StaticMeshImport = asset.importMap()[-objectReference.pakValue() - 1];

    if (!StaticMeshImport.IsPackageImport()) {
        throw std::runtime_error("!StaticMeshImport.IsPackageImport() not implemented!");
    }

    const auto& ref = StaticMeshImport.ToPackageImportRef();
    return meshManager_->loadMesh(asset.importedPackageNames().at(ref.GetImportedPackageIndex()).toString(),
        asset.importedPublicExportHashes().at(ref.GetImportedPublicExportHashIndex()));
}

Satisfactory3DMap::ModelManager::MeshInfo Satisfactory3DMap::ModelManager::getStaticMeshTransformFromStruct(
    s::AssetFile& asset, const std::shared_ptr<s::Struct>& instanceDataStruct) {

    const auto* instanceData = dynamic_cast<s::PropertyStruct*>(instanceDataStruct.get());
    if (instanceData == nullptr) {
        throw std::runtime_error("Unexpected type!");
    }

    const auto& staticMeshRef = instanceData->Data.get<s::ObjectProperty>("StaticMesh").Value;
    auto mesh = readStaticMeshFromReference(asset, staticMeshRef);

    glm::mat4 modelMx = glm::mat4(1.0f);
    try {
        const auto& relativeTransform = instanceData->Data.get<s::StructProperty>("RelativeTransform");
        const auto* relativeTransformStruct = dynamic_cast<s::PropertyStruct*>(relativeTransform.Value.get());
        if (relativeTransformStruct == nullptr) {
            throw std::runtime_error("Bad struct type!");
        }

        const auto* Rotation = dynamic_cast<const s::QuatStruct*>(
            relativeTransformStruct->Data.get<s::StructProperty>("Rotation").Value.get());
        const auto* Translation = dynamic_cast<const s::VectorStruct*>(
            relativeTransformStruct->Data.get<s::StructProperty>("Translation").Value.get());
        const auto* Scale3D = dynamic_cast<const s::VectorStruct*>(
            relativeTransformStruct->Data.get<s::StructProperty>("Scale3D").Value.get());
        if (Rotation == nullptr || Translation == nullptr || Scale3D == nullptr) {
            throw std::runtime_error("Bad struct types!");
        }

        const glm::mat4 rotationMx = glm::mat4_cast(glmCast(Rotation->Data));
        const glm::mat4 translationMx = glm::translate(glm::mat4(1.0f), glmCast(Translation->Data));
        const glm::mat4 scaleMx = glm::scale(glm::mat4(1.0f), glmCast(Scale3D->Data));

        modelMx = translationMx * rotationMx * scaleMx;
    } catch (...) {}

    return {mesh, modelMx};
}
