#pragma once

#include <cstdint>
#include <memory>
#include <string>
#include <vector>

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include "SatisfactorySave/GameTypes/FactoryGame/FGConveyorChainTypes.h"
#include "SatisfactorySave/GameTypes/FactoryGame/FGConveyorItem.h"
#include "SatisfactorySave/GameTypes/FactoryGame/FGDroneVehicle.h"
#include "SatisfactorySave/GameTypes/FactoryGame/FGLightweightBuildableSubsystem.h"
#include "SatisfactorySave/GameTypes/FactoryGame/FGObjectReference.h"
#include "SatisfactorySave/GameTypes/FactoryGame/FGVehicle.h"
#include "SatisfactorySave/GameTypes/FactoryGamePlugins/OnlineIntegration/LocalUserInfo.h"
#include "SatisfactorySave/GameTypes/Save/SaveGame.h"
#include "SatisfactorySave/GameTypes/Structs/Base/Struct.h"
#include "SatisfactorySave/GameTypes/UE/Core/UObject/NameTypes.h"
#include "SatisfactorySave/GameTypes/UE/Core/UObject/ScriptDelegates.h"
#include "SatisfactorySave/GameTypes/UE/CoreUObject/UObject/SoftObjectPath.h"
#include "SatisfactorySave/GameTypes/UE/Engine/Components/SplineComponent.h"

namespace py = pybind11;
namespace s = SatisfactorySave;

void init_vector(py::module_& m);
void init_tmap(py::module_& m);

void init_GameTypes_Arrays(py::module_& m);
void init_GameTypes_FactoryGame(py::module_& m);
void init_GameTypes_FactoryGamePlugins(py::module_& m);
void init_GameTypes_MapTypes(py::module_& m);
void init_GameTypes_Mods(py::module_& m);
void init_GameTypes_Properties(py::module_& m);
void init_GameTypes_Save(py::module_& m);
void init_GameTypes_Sets(py::module_& m);
void init_GameTypes_Structs(py::module_& m);
void init_GameTypes_UE(py::module_& m);
void init_GameTypes_UObjects(py::module_& m);

inline void init_all(py::module_& m) {
    init_vector(m);
    init_tmap(m);

    init_GameTypes_UE(m);
    init_GameTypes_FactoryGamePlugins(m);
    init_GameTypes_FactoryGame(m);
    init_GameTypes_Mods(m);

    init_GameTypes_Arrays(m);
    init_GameTypes_MapTypes(m);
    init_GameTypes_Sets(m);
    init_GameTypes_Structs(m);

    init_GameTypes_Properties(m);
    init_GameTypes_UObjects(m);
    init_GameTypes_Save(m);
}

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
PYBIND11_MAKE_OPAQUE(std::vector<s::ScriptDelegate>);
PYBIND11_MAKE_OPAQUE(std::vector<s::FConveyorBeltItem>);
PYBIND11_MAKE_OPAQUE(std::vector<s::FConveyorChainSplineSegment>);
PYBIND11_MAKE_OPAQUE(std::vector<s::FDroneAction>);
PYBIND11_MAKE_OPAQUE(std::vector<s::FRuntimeBuildableInstanceData>);
PYBIND11_MAKE_OPAQUE(std::vector<s::FSplinePointData>);
PYBIND11_MAKE_OPAQUE(std::vector<s::FVehiclePhysicsData>);
PYBIND11_MAKE_OPAQUE(s::SaveObjectList);
PYBIND11_MAKE_OPAQUE(std::vector<std::shared_ptr<s::Struct>>);
PYBIND11_MAKE_OPAQUE(std::vector<s::FLocalUserNetIdBundle>);
