#include "SatisfactorySave/GameTypes/FactoryGame/Buildables/FGBuildableConveyorBase.h"
#include "SatisfactorySave/GameTypes/FactoryGame/Buildables/FGBuildableWire.h"
#include "SatisfactorySave/GameTypes/FactoryGame/FGCircuitSubsystem.h"
#include "SatisfactorySave/GameTypes/FactoryGame/FGConveyorChainActor.h"
#include "SatisfactorySave/GameTypes/FactoryGame/FGDroneVehicle.h"
#include "SatisfactorySave/GameTypes/FactoryGame/FGGameMode.h"
#include "SatisfactorySave/GameTypes/FactoryGame/FGGameState.h"
#include "SatisfactorySave/GameTypes/FactoryGame/FGLightweightBuildableSubsystem.h"
#include "SatisfactorySave/GameTypes/FactoryGame/FGPlayerState.h"
#include "SatisfactorySave/GameTypes/FactoryGame/FGRailroadVehicle.h"
#include "SatisfactorySave/GameTypes/FactoryGame/FGVehicle.h"
#include "SatisfactorySave/GameTypes/UE/CoreUObject/UObject/Object.h"
#include "SatisfactorySave/GameTypes/UE/Engine/Components/ActorComponent.h"
#include "SatisfactorySave/GameTypes/UE/Engine/GameFramework/Actor.h"
#include "libsavepy_common.h"

void init_GameTypes_UObjects(py::module_& m) {
    py::classh<s::UObject>(m, "UObject")
        .def(py::init<>())
        .def_readwrite("Properties", &s::UObject::Properties)
        .def_readwrite("Guid", &s::UObject::Guid);

    py::classh<s::AActor, s::UObject>(m, "AActor")
        .def(py::init<>())
        .def_readwrite("Owner", &s::AActor::Owner)
        .def_readwrite("Components", &s::AActor::Components);

    py::classh<s::AFGBuildableConveyorBase, s::AActor>(m, "AFGBuildableConveyorBase")
        .def(py::init<>())
        .def_readwrite("mItems", &s::AFGBuildableConveyorBase::mItems);

    py::classh<s::AFGConveyorChainActor, s::AActor>(m, "AFGConveyorChainActor")
        .def(py::init<>())
        .def_readwrite("mFirstConveyor", &s::AFGConveyorChainActor::mFirstConveyor)
        .def_readwrite("mLastConveyor", &s::AFGConveyorChainActor::mLastConveyor)
        .def_readwrite("mChainSplineSegments", &s::AFGConveyorChainActor::mChainSplineSegments)
        .def_readwrite("mTotalLength", &s::AFGConveyorChainActor::mTotalLength)
        .def_readwrite("mNumItems", &s::AFGConveyorChainActor::mNumItems)
        .def_readwrite("mLeadItemIndex", &s::AFGConveyorChainActor::mLeadItemIndex)
        .def_readwrite("mTailItemIndex", &s::AFGConveyorChainActor::mTailItemIndex)
        .def_readwrite("mConveyorChainItems", &s::AFGConveyorChainActor::mConveyorChainItems);

    py::classh<s::AFGBuildableWire, s::AActor>(m, "AFGBuildableWire")
        .def(py::init<>())
        .def_readwrite("mConnections", &s::AFGBuildableWire::mConnections);

    py::classh<s::AFGCircuitSubsystem, s::AActor>(m, "AFGCircuitSubsystem")
        .def(py::init<>())
        .def_readwrite("mCircuits", &s::AFGCircuitSubsystem::mCircuits);

    py::classh<s::AFGLightweightBuildableSubsystem, s::AActor>(m, "AFGLightweightBuildableSubsystem")
        .def(py::init<>())
        .def_readwrite("currentLightweightVersion", &s::AFGLightweightBuildableSubsystem::currentLightweightVersion)
        .def_readwrite("mBuildableClassToInstanceArray", &s::AFGLightweightBuildableSubsystem::mBuildableClassToInstanceArray);

    py::classh<s::AFGGameMode, s::AActor>(m, "AFGGameMode")
        .def(py::init<>())
        .def_readwrite("rawPlayerStatePointers", &s::AFGGameMode::rawPlayerStatePointers);

    py::classh<s::AFGGameState, s::AActor>(m, "AFGGameState")
        .def(py::init<>())
        .def_readwrite("rawPlayerStatePointers", &s::AFGGameState::rawPlayerStatePointers);

    py::classh<s::AFGPlayerState, s::AActor>(m, "AFGPlayerState")
        .def(py::init<>())
        .def_readwrite("Id", &s::AFGPlayerState::Id);

    py::classh<s::AFGVehicle, s::AActor>(m, "AFGVehicle")
        .def(py::init<>())
        .def_readwrite("mStoredPhysicsData", &s::AFGVehicle::mStoredPhysicsData);

    py::classh<s::AFGRailroadVehicle, s::AFGVehicle>(m, "AFGRailroadVehicle")
        .def(py::init<>())
        .def_readwrite("mCoupledVehicleFront", &s::AFGRailroadVehicle::mCoupledVehicleFront)
        .def_readwrite("mCoupledVehicleBack", &s::AFGRailroadVehicle::mCoupledVehicleBack);

    py::classh<s::AFGDroneVehicle, s::AFGVehicle>(m, "AFGDroneVehicle")
        .def(py::init<>())
        .def_readwrite("mActiveAction", &s::AFGDroneVehicle::mActiveAction)
        .def_readwrite("mActionQueue", &s::AFGDroneVehicle::mActionQueue);

    py::classh<s::UActorComponent, s::UObject>(m, "UActorComponent")
        .def(py::init<>());
}
