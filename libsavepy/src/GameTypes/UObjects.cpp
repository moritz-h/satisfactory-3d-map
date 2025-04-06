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
    py::class_<s::UObject, std::shared_ptr<s::UObject>>(m, "UObject")
        .def(py::init<>())
        .def_readwrite("Properties", &s::UObject::Properties)
        .def_readwrite("Guid", &s::UObject::Guid);

    py::class_<s::AActor, s::UObject, std::shared_ptr<s::AActor>>(m, "AActor")
        .def(py::init<>())
        .def_readwrite("Owner", &s::AActor::Owner)
        .def_readwrite("Components", &s::AActor::Components);

    py::class_<s::AFGBuildableConveyorBase, s::AActor, std::shared_ptr<s::AFGBuildableConveyorBase>>(m,
        "AFGBuildableConveyorBase")
        .def(py::init<>())
        .def_readwrite("mItems", &s::AFGBuildableConveyorBase::mItems);

    py::class_<s::AFGConveyorChainActor, s::AActor, std::shared_ptr<s::AFGConveyorChainActor>>(m,
        "AFGConveyorChainActor")
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

    py::class_<s::AFGCircuitSubsystem, s::AActor, std::shared_ptr<s::AFGCircuitSubsystem>>(m, "AFGCircuitSubsystem")
        .def(py::init<>())
        .def_readwrite("mCircuits", &s::AFGCircuitSubsystem::mCircuits);

    py::class_<s::AFGLightweightBuildableSubsystem, s::AActor, std::shared_ptr<s::AFGLightweightBuildableSubsystem>>(m,
        "AFGLightweightBuildableSubsystem")
        .def(py::init<>())
        .def_readwrite("currentLightweightVersion", &s::AFGLightweightBuildableSubsystem::currentLightweightVersion)
        .def_readwrite("mBuildableClassToInstanceArray",
            &s::AFGLightweightBuildableSubsystem::mBuildableClassToInstanceArray);

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

    py::class_<s::UActorComponent, s::UObject, std::shared_ptr<s::UActorComponent>>(m, "UActorComponent")
        .def(py::init<>());
}
