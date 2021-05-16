#include "ModelManager.h"

#include "SaveGame/Objects/SaveActor.h"
#include "Utils/StringUtils.h"

Satisfactory3DMap::ModelManager::ModelManager() {

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
        models_.emplace_back(std::make_unique<Model>(entry.first));
        modelSavePaths_.emplace_back(entry.second);
    }
    for (const auto& entry : splineModels) {
        splineModels_.emplace_back(std::make_unique<Model>(entry.first));
        splineModelSavePaths_.emplace_back(entry.second);
    }
}

std::pair<Satisfactory3DMap::ModelManager::ModelType, int32_t> Satisfactory3DMap::ModelManager::classifyActor(
    const SaveActor& a) {

    const auto& className = a.className();

    for (int32_t i = 0; i < splineModels_.size(); i++) {
        for (const auto& name : splineModelSavePaths_[i]) {
            if (className == name) {
                return std::make_pair(ModelType::SplineModel, i);
            }
        }
    }

    // model idx 0 is fallback model, start search with i = 1.
    for (int32_t i = 1; i < models_.size(); i++) {
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
