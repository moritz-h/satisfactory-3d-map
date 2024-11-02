#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include "SatisfactorySave/GameTypes/UE/Satisfactory/Buildables/FGBuildableConveyorBase.h"
#include "SatisfactorySave/GameTypes/UE/Satisfactory/Buildables/FGBuildableWire.h"
#include "SatisfactorySave/GameTypes/UE/Satisfactory/FGCircuitSubsystem.h"
#include "SatisfactorySave/GameTypes/UE/Satisfactory/FGConveyorChainActor.h"
#include "SatisfactorySave/GameTypes/UE/Satisfactory/FGDroneVehicle.h"
#include "SatisfactorySave/GameTypes/UE/Satisfactory/FGGameMode.h"
#include "SatisfactorySave/GameTypes/UE/Satisfactory/FGGameState.h"
#include "SatisfactorySave/GameTypes/UE/Satisfactory/FGLightweightBuildableSubsystem.h"
#include "SatisfactorySave/GameTypes/UE/Satisfactory/FGPlayerState.h"
#include "SatisfactorySave/GameTypes/UE/Satisfactory/FGRailroadVehicle.h"
#include "SatisfactorySave/GameTypes/UE/Satisfactory/FGVehicle.h"
#include "libsavepy_common.h"

namespace py = pybind11;
namespace s = SatisfactorySave;

void init_GameTypes_UE_SatisfactoryObjects(py::module_& m) {
    py::class_<s::AFGBuildableConveyorBase, s::AActor, std::shared_ptr<s::AFGBuildableConveyorBase>>(m, "AFGBuildableConveyorBase")
        .def(py::init<>())
        .def_readwrite("mItems", &s::AFGBuildableConveyorBase::mItems);

    py::class_<s::AFGConveyorChainActor, s::AActor, std::shared_ptr<s::AFGConveyorChainActor>>(m, "AFGConveyorChainActor")
        .def(py::init<>())
        .def_readwrite("mFirstConveyor", &s::AFGConveyorChainActor::mFirstConveyor)
        .def_readwrite("mLastConveyor", &s::AFGConveyorChainActor::mLastConveyor)
        .def_readwrite("mChainSplineSegments", &s::AFGConveyorChainActor::mChainSplineSegments)
        .def_readwrite("mTotalLength", &s::AFGConveyorChainActor::mTotalLength)
        .def_readwrite("mNumItems", &s::AFGConveyorChainActor::mNumItems)
        .def_readwrite("mLeadItemIndex", &s::AFGConveyorChainActor::mLeadItemIndex)
        .def_readwrite("mTailItemIndex", &s::AFGConveyorChainActor::mTailItemIndex)
        .def_readwrite("mConveyorChainItems", &s::AFGConveyorChainActor::mConveyorChainItems);

    py::class_<s::AFGBuildableWire, s::AActor, std::shared_ptr<s::AFGBuildableWire>>(m, "AFGBuildableWire")
        .def(py::init<>())
        .def_readwrite("mConnections", &s::AFGBuildableWire::mConnections);

    // AFGCircuitSubsystem::mCircuits
    py::class_<s::TMap<uint32_t, s::FObjectReferenceDisc>>(m, "Map<uint32,FObjectReferenceDisc>")
        .def_readwrite("Keys", &s::TMap<uint32_t, s::FObjectReferenceDisc>::Keys)
        .def_readwrite("Values", &s::TMap<uint32_t, s::FObjectReferenceDisc>::Values);

    py::class_<s::AFGCircuitSubsystem, s::AActor, std::shared_ptr<s::AFGCircuitSubsystem>>(m, "AFGCircuitSubsystem")
        .def(py::init<>())
        .def_readwrite("mCircuits", &s::AFGCircuitSubsystem::mCircuits);

    // AFGLightweightBuildableSubsystem::mBuildableClassToInstanceArray
    py::class_<s::TMap<s::FObjectReferenceDisc, std::vector<s::FRuntimeBuildableInstanceData>>>(m, "Map<FObjectReferenceDisc,vector<FRuntimeBuildableInstanceData>>")
        .def_readwrite("Keys", &s::TMap<s::FObjectReferenceDisc, std::vector<s::FRuntimeBuildableInstanceData>>::Keys)
        .def_readwrite("Values", &s::TMap<s::FObjectReferenceDisc, std::vector<s::FRuntimeBuildableInstanceData>>::Values);

    py::class_<s::AFGLightweightBuildableSubsystem, s::AActor, std::shared_ptr<s::AFGLightweightBuildableSubsystem>>(m, "AFGLightweightBuildableSubsystem")
        .def(py::init<>())
        .def_readwrite("mBuildableClassToInstanceArray", &s::AFGLightweightBuildableSubsystem::mBuildableClassToInstanceArray);

    py::class_<s::AFGGameMode, s::AActor, std::shared_ptr<s::AFGGameMode>>(m, "AFGGameMode")
        .def(py::init<>())
        .def_readwrite("rawPlayerStatePointers", &s::AFGGameMode::rawPlayerStatePointers);

    py::class_<s::AFGGameState, s::AActor, std::shared_ptr<s::AFGGameState>>(m, "AFGGameState")
        .def(py::init<>())
        .def_readwrite("rawPlayerStatePointers", &s::AFGGameState::rawPlayerStatePointers);

    py::class_<s::AFGPlayerState, s::AActor, std::shared_ptr<s::AFGPlayerState>>(m, "AFGPlayerState")
        .def(py::init<>())
        .def_readwrite("Id", &s::AFGPlayerState::Id);

    py::class_<s::AFGVehicle, s::AActor, std::shared_ptr<s::AFGVehicle>>(m, "AFGVehicle")
        .def(py::init<>())
        .def_readwrite("mStoredPhysicsData", &s::AFGVehicle::mStoredPhysicsData);

    py::class_<s::AFGRailroadVehicle, s::AFGVehicle, std::shared_ptr<s::AFGRailroadVehicle>>(m, "AFGRailroadVehicle")
        .def(py::init<>())
        .def_readwrite("mCoupledVehicleFront", &s::AFGRailroadVehicle::mCoupledVehicleFront)
        .def_readwrite("mCoupledVehicleBack", &s::AFGRailroadVehicle::mCoupledVehicleBack);

    py::class_<s::AFGDroneVehicle, s::AFGVehicle, std::shared_ptr<s::AFGDroneVehicle>>(m, "AFGDroneVehicle")
        .def(py::init<>())
        .def_readwrite("mActiveAction", &s::AFGDroneVehicle::mActiveAction)
        .def_readwrite("mActionQueue", &s::AFGDroneVehicle::mActionQueue);
}
