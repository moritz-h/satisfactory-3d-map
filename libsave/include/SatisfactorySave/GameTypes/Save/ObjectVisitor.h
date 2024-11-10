#pragma once

#include <typeinfo>

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
#include "satisfactorysave_export.h"

namespace SatisfactorySave {

    class SATISFACTORYSAVE_API ObjectVisitor {
    public:
        virtual ~ObjectVisitor() = default;

        virtual void visit(UObject& o) = 0;
        virtual void visit(AActor& o) = 0;
        virtual void visit(AFGBuildableConveyorBase& o) = 0;
        virtual void visit(AFGConveyorChainActor& o) = 0;
        virtual void visit(AFGBuildableWire& o) = 0;
        virtual void visit(AFGCircuitSubsystem& o) = 0;
        virtual void visit(AFGLightweightBuildableSubsystem& o) = 0;
        virtual void visit(AFGGameMode& o) = 0;
        virtual void visit(AFGGameState& o) = 0;
        virtual void visit(AFGPlayerState& o) = 0;
        virtual void visit(AFGVehicle& o) = 0;
        virtual void visit(AFGRailroadVehicle& o) = 0;
        virtual void visit(AFGDroneVehicle& o) = 0;
        virtual void visit(UActorComponent& o) = 0;

        void dispatch(UObject& obj) {
            if (typeid(obj) == typeid(UObject)) {
                visit(static_cast<UObject&>(obj));
            } else if (typeid(obj) == typeid(AActor)) {
                visit(static_cast<AActor&>(obj));
            } else if (typeid(obj) == typeid(AFGBuildableConveyorBase)) {
                visit(static_cast<AFGBuildableConveyorBase&>(obj));
            } else if (typeid(obj) == typeid(AFGConveyorChainActor)) {
                visit(static_cast<AFGConveyorChainActor&>(obj));
            } else if (typeid(obj) == typeid(AFGBuildableWire)) {
                visit(static_cast<AFGBuildableWire&>(obj));
            } else if (typeid(obj) == typeid(AFGCircuitSubsystem)) {
                visit(static_cast<AFGCircuitSubsystem&>(obj));
            } else if (typeid(obj) == typeid(AFGLightweightBuildableSubsystem)) {
                visit(static_cast<AFGLightweightBuildableSubsystem&>(obj));
            } else if (typeid(obj) == typeid(AFGGameMode)) {
                visit(static_cast<AFGGameMode&>(obj));
            } else if (typeid(obj) == typeid(AFGGameState)) {
                visit(static_cast<AFGGameState&>(obj));
            } else if (typeid(obj) == typeid(AFGPlayerState)) {
                visit(static_cast<AFGPlayerState&>(obj));
            } else if (typeid(obj) == typeid(AFGVehicle)) {
                visit(static_cast<AFGVehicle&>(obj));
            } else if (typeid(obj) == typeid(AFGRailroadVehicle)) {
                visit(static_cast<AFGRailroadVehicle&>(obj));
            } else if (typeid(obj) == typeid(AFGDroneVehicle)) {
                visit(static_cast<AFGDroneVehicle&>(obj));
            } else if (typeid(obj) == typeid(UActorComponent)) {
                visit(static_cast<UActorComponent&>(obj));
            } else {
                AActor* actor = dynamic_cast<AActor*>(&obj);
                if (actor != nullptr) {
                    visit(*actor);
                } else {
                    visit(static_cast<UObject&>(obj));
                }
            }
        }
    };
} // namespace SatisfactorySave
