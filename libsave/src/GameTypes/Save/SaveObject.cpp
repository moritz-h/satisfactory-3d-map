#include "GameTypes/Save/SaveObject.h"

#include <unordered_set>

#include <spdlog/spdlog.h>

#include "GameTypes/FactoryGame/Buildables/FGBuildableConveyorBase.h"
#include "GameTypes/FactoryGame/Buildables/FGBuildableWire.h"
#include "GameTypes/FactoryGame/FGCircuitSubsystem.h"
#include "GameTypes/FactoryGame/FGConveyorChainActor.h"
#include "GameTypes/FactoryGame/FGDroneVehicle.h"
#include "GameTypes/FactoryGame/FGGameMode.h"
#include "GameTypes/FactoryGame/FGGameState.h"
#include "GameTypes/FactoryGame/FGLightweightBuildableSubsystem.h"
#include "GameTypes/FactoryGame/FGPlayerState.h"
#include "GameTypes/FactoryGame/FGRailroadVehicle.h"
#include "GameTypes/FactoryGame/FGVehicle.h"
#include "GameTypes/UE/Engine/Components/ActorComponent.h"
#include "GameTypes/UE/Engine/GameFramework/Actor.h"
#include "IO/Archive/IStreamArchive.h"
#include "IO/Archive/OStreamArchive.h"

std::shared_ptr<SatisfactorySave::UObject> SatisfactorySave::UObjectFactory(bool is_actor,
    const std::string& class_name) {
    if (is_actor) {
        if (class_name ==
                "/Game/FactoryGame/Buildable/Factory/ConveyorBeltMk1/Build_ConveyorBeltMk1.Build_ConveyorBeltMk1_C" ||
            class_name ==
                "/Game/FactoryGame/Buildable/Factory/ConveyorBeltMk2/Build_ConveyorBeltMk2.Build_ConveyorBeltMk2_C" ||
            class_name ==
                "/Game/FactoryGame/Buildable/Factory/ConveyorBeltMk3/Build_ConveyorBeltMk3.Build_ConveyorBeltMk3_C" ||
            class_name ==
                "/Game/FactoryGame/Buildable/Factory/ConveyorBeltMk4/Build_ConveyorBeltMk4.Build_ConveyorBeltMk4_C" ||
            class_name ==
                "/Game/FactoryGame/Buildable/Factory/ConveyorBeltMk5/Build_ConveyorBeltMk5.Build_ConveyorBeltMk5_C" ||
            class_name ==
                "/Game/FactoryGame/Buildable/Factory/ConveyorBeltMk6/Build_ConveyorBeltMk6.Build_ConveyorBeltMk6_C" ||
            class_name ==
                "/Game/FactoryGame/Buildable/Factory/ConveyorLiftMk1/Build_ConveyorLiftMk1.Build_ConveyorLiftMk1_C" ||
            class_name ==
                "/Game/FactoryGame/Buildable/Factory/ConveyorLiftMk2/Build_ConveyorLiftMk2.Build_ConveyorLiftMk2_C" ||
            class_name ==
                "/Game/FactoryGame/Buildable/Factory/ConveyorLiftMk3/Build_ConveyorLiftMk3.Build_ConveyorLiftMk3_C" ||
            class_name ==
                "/Game/FactoryGame/Buildable/Factory/ConveyorLiftMk4/Build_ConveyorLiftMk4.Build_ConveyorLiftMk4_C" ||
            class_name ==
                "/Game/FactoryGame/Buildable/Factory/ConveyorLiftMk5/Build_ConveyorLiftMk5.Build_ConveyorLiftMk5_C" ||
            class_name ==
                "/Game/FactoryGame/Buildable/Factory/ConveyorLiftMk6/Build_ConveyorLiftMk6.Build_ConveyorLiftMk6_C") {
            return std::make_shared<AFGBuildableConveyorBase>();
        } else if (class_name == "/Script/FactoryGame.FGConveyorChainActor" ||
                   class_name == "/Script/FactoryGame.FGConveyorChainActor_RepSizeHuge" ||
                   class_name == "/Script/FactoryGame.FGConveyorChainActor_RepSizeLarge" ||
                   class_name == "/Script/FactoryGame.FGConveyorChainActor_RepSizeMedium" ||
                   class_name == "/Script/FactoryGame.FGConveyorChainActor_RepSizeNoCull") {
            return std::make_shared<AFGConveyorChainActor>();
        } else if (class_name == "/Game/FactoryGame/Buildable/Factory/PowerLine/Build_PowerLine.Build_PowerLine_C" ||
                   class_name == "/Game/FactoryGame/Events/Christmas/Buildings/PowerLineLights/Build_XmassLightsLine.Build_XmassLightsLine_C") {
            return std::make_shared<AFGBuildableWire>();
        } else if (class_name == "/Game/FactoryGame/-Shared/Blueprint/BP_CircuitSubsystem.BP_CircuitSubsystem_C") {
            return std::make_shared<AFGCircuitSubsystem>();
        } else if (class_name == "/Script/FactoryGame.FGLightweightBuildableSubsystem") {
            return std::make_shared<AFGLightweightBuildableSubsystem>();
        } else if (class_name == "/Game/FactoryGame/-Shared/Blueprint/BP_GameMode.BP_GameMode_C") {
            return std::make_shared<AFGGameMode>();
        } else if (class_name == "/Game/FactoryGame/-Shared/Blueprint/BP_GameState.BP_GameState_C") {
            return std::make_shared<AFGGameState>();
        } else if (class_name == "/Game/FactoryGame/Character/Player/BP_PlayerState.BP_PlayerState_C") {
            return std::make_shared<AFGPlayerState>();
        } else if (class_name == "/Game/FactoryGame/Buildable/Vehicle/Cyberwagon/Testa_BP_WB.Testa_BP_WB_C" ||
                   class_name == "/Game/FactoryGame/Buildable/Vehicle/Explorer/BP_Explorer.BP_Explorer_C" ||
                   class_name == "/Game/FactoryGame/Buildable/Vehicle/Golfcart/BP_Golfcart.BP_Golfcart_C" ||
                   class_name == "/Game/FactoryGame/Buildable/Vehicle/Golfcart/BP_GolfcartGold.BP_GolfcartGold_C" ||
                   class_name == "/Game/FactoryGame/Buildable/Vehicle/Tractor/BP_Tractor.BP_Tractor_C" ||
                   class_name == "/Game/FactoryGame/Buildable/Vehicle/Truck/BP_Truck.BP_Truck_C") {
            return std::make_shared<AFGVehicle>();
        } else if (class_name == "/Game/FactoryGame/Buildable/Vehicle/Train/Locomotive/BP_Locomotive.BP_Locomotive_C" ||
                   class_name == "/Game/FactoryGame/Buildable/Vehicle/Train/Wagon/BP_FreightWagon.BP_FreightWagon_C") {
            return std::make_shared<AFGRailroadVehicle>();
        } else if (class_name ==
                   "/Game/FactoryGame/Buildable/Factory/DroneStation/BP_DroneTransport.BP_DroneTransport_C") {
            return std::make_shared<AFGDroneVehicle>();
        } else {
            return std::make_shared<AActor>();
        }
    } else {
        if (class_name == "/Script/FactoryGame.FGDroneMovementComponent" ||
            class_name == "/Script/FactoryGame.FGFactoryConnectionComponent" ||
            class_name == "/Script/FactoryGame.FGFactoryLegsComponent" ||
            class_name == "/Script/FactoryGame.FGHealthComponent" ||
            class_name == "/Script/FactoryGame.FGInventoryComponent" ||
            class_name == "/Script/FactoryGame.FGInventoryComponentEquipment" ||
            class_name == "/Script/FactoryGame.FGInventoryComponentTrash" ||
            class_name == "/Script/FactoryGame.FGPipeConnectionComponent" ||
            class_name == "/Script/FactoryGame.FGPipeConnectionComponentHyper" ||
            class_name == "/Script/FactoryGame.FGPipeConnectionFactory" ||
            class_name == "/Script/FactoryGame.FGPowerConnectionComponent" ||
            class_name == "/Script/FactoryGame.FGPowerInfoComponent" ||
            class_name == "/Script/FactoryGame.FGRailroadTrackConnectionComponent" ||
            class_name == "/Script/FactoryGame.FGShoppingListComponent" ||
            class_name == "/Script/FactoryGame.FGTrainPlatformConnection") {
            return std::make_shared<UActorComponent>();
        } else {
            return std::make_shared<UObject>();
        }
    }
}

void SatisfactorySave::SaveObject::serializeTOC(Archive& ar) {
    if (ar.isIArchive()) {
        bool isActor = false;
        ar << isActor;
        if (isActor) {
            Header.emplace<FActorSaveHeader>();
            ar << std::get<FActorSaveHeader>(Header);
        } else {
            Header.emplace<FObjectSaveHeader>();
            ar << std::get<FObjectSaveHeader>(Header);
        }
    } else {
        bool isActor = std::holds_alternative<FActorSaveHeader>(Header);
        ar << isActor;
        if (isActor) {
            ar << std::get<FActorSaveHeader>(Header);
        } else {
            ar << std::get<FObjectSaveHeader>(Header);
        }
    }
}

void SatisfactorySave::SaveObject::serializeData(Archive& ar, bool data_header) {
    if (ar.isIArchive()) {
        Object = UObjectFactory(isActor(), baseHeader().ClassName);
    }

    std::unique_ptr<StackGuard<int32_t>> save_version_stack_pusher;
    if (data_header) {
        ar << SaveVersion;
        ar << ShouldMigrateObjectRefsToPersistent;
        save_version_stack_pusher = ar.pushSaveVersion(SaveVersion);
    }

    if (ar.isIArchive()) {
        auto& inAr = dynamic_cast<IStreamArchive&>(ar);

        // Required, because some properties have no type information.
        auto parent_info_stack_pusher = inAr.pushParentClassInfo(baseHeader().ClassName);

        // Check stream pos to validate parser.
        const auto length = inAr.read<int32_t>();

        auto pos_before = inAr.tell();
        inAr << *Object;
        auto pos_after = inAr.tell();

        // Read unknown class data as binary buffer
        if (pos_after - pos_before != length) {
            BinaryClassData = inAr.read_buffer(length - (pos_after - pos_before));
            static std::unordered_set<std::string> logged_warnings; // Prevent log spam.
            if (!logged_warnings.contains(baseHeader().ClassName)) {
                logged_warnings.insert(baseHeader().ClassName);
                spdlog::warn("Class {} (isActor: {}) has unknown binary data!", baseHeader().ClassName, isActor());
            }
        }

    } else {
        auto& outAr = dynamic_cast<OStreamArchive&>(ar);

        auto pos_size = outAr.tell();
        outAr.write<int32_t>(0);

        auto pos_before = outAr.tell();
        outAr << *Object;
        if (!BinaryClassData.empty()) {
            outAr.write_buffer(BinaryClassData);
        }
        auto pos_after = outAr.tell();

        outAr.seek(pos_size);
        outAr.write(static_cast<int32_t>(pos_after - pos_before));
        outAr.seek(pos_after);
    }
}
