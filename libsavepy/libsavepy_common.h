#include <cstdint>
#include <memory>
#include <string>
#include <vector>

#include <pybind11/pybind11.h>

#include "SatisfactorySave/GameTypes/FactoryGame/FGConveyorChainTypes.h"
#include "SatisfactorySave/GameTypes/FactoryGame/FGConveyorItem.h"
#include "SatisfactorySave/GameTypes/FactoryGame/FGDroneVehicle.h"
#include "SatisfactorySave/GameTypes/FactoryGame/FGLightweightBuildableSubsystem.h"
#include "SatisfactorySave/GameTypes/FactoryGame/FGObjectReference.h"
#include "SatisfactorySave/GameTypes/FactoryGame/FGVehicle.h"
#include "SatisfactorySave/GameTypes/Save/SaveGame.h"
#include "SatisfactorySave/GameTypes/Structs/Base/Struct.h"
#include "SatisfactorySave/GameTypes/UE/Core/UObject/NameTypes.h"
#include "SatisfactorySave/GameTypes/UE/CoreUObject/UObject/SoftObjectPath.h"
#include "SatisfactorySave/GameTypes/UE/Engine/Components/SplineComponent.h"

namespace s = SatisfactorySave;

PYBIND11_MAKE_OPAQUE(std::vector<int8_t>);
PYBIND11_MAKE_OPAQUE(std::vector<int16_t>);
PYBIND11_MAKE_OPAQUE(std::vector<int32_t>);
PYBIND11_MAKE_OPAQUE(std::vector<int64_t>);
PYBIND11_MAKE_OPAQUE(std::vector<uint8_t>);
PYBIND11_MAKE_OPAQUE(std::vector<uint16_t>);
PYBIND11_MAKE_OPAQUE(std::vector<uint32_t>);
PYBIND11_MAKE_OPAQUE(std::vector<uint64_t>);
PYBIND11_MAKE_OPAQUE(std::vector<float>);
PYBIND11_MAKE_OPAQUE(std::vector<double>);
PYBIND11_MAKE_OPAQUE(std::vector<std::string>);
PYBIND11_MAKE_OPAQUE(std::vector<s::FName>);
PYBIND11_MAKE_OPAQUE(std::vector<s::FObjectReferenceDisc>);
PYBIND11_MAKE_OPAQUE(std::vector<s::FSoftObjectPath>);
PYBIND11_MAKE_OPAQUE(std::vector<s::FConveyorBeltItem>);
PYBIND11_MAKE_OPAQUE(std::vector<s::FConveyorChainSplineSegment>);
PYBIND11_MAKE_OPAQUE(std::vector<s::FDroneAction>);
PYBIND11_MAKE_OPAQUE(std::vector<s::FRuntimeBuildableInstanceData>);
PYBIND11_MAKE_OPAQUE(std::vector<s::FSplinePointData>);
PYBIND11_MAKE_OPAQUE(std::vector<s::FVehiclePhysicsData>);
PYBIND11_MAKE_OPAQUE(s::SaveObjectList);
PYBIND11_MAKE_OPAQUE(std::vector<std::shared_ptr<s::Struct>>);
