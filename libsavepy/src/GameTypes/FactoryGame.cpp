#include "SatisfactorySave/GameTypes/FactoryGame/Buildables/FGBuildableRailroadTrack.h"
#include "SatisfactorySave/GameTypes/FactoryGame/FGActorSaveHeaderTypes.h"
#include "SatisfactorySave/GameTypes/FactoryGame/FGConveyorChainTypes.h"
#include "SatisfactorySave/GameTypes/FactoryGame/FGConveyorItem.h"
#include "SatisfactorySave/GameTypes/FactoryGame/FGDroneVehicle.h"
#include "SatisfactorySave/GameTypes/FactoryGame/FGDynamicStruct.h"
#include "SatisfactorySave/GameTypes/FactoryGame/FGFactoryBlueprintTypes.h"
#include "SatisfactorySave/GameTypes/FactoryGame/FGFactoryColoringTypes.h"
#include "SatisfactorySave/GameTypes/FactoryGame/FGFluidIntegrantInterface.h"
#include "SatisfactorySave/GameTypes/FactoryGame/FGIconLibrary.h"
#include "SatisfactorySave/GameTypes/FactoryGame/FGInventoryComponent.h"
#include "SatisfactorySave/GameTypes/FactoryGame/FGLightweightBuildableSubsystem.h"
#include "SatisfactorySave/GameTypes/FactoryGame/FGObjectReference.h"
#include "SatisfactorySave/GameTypes/FactoryGame/FGSaveManagerInterface.h"
#include "SatisfactorySave/GameTypes/FactoryGame/FGVehicle.h"
#include "SatisfactorySave/GameTypes/FactoryGame/FWPSaveDataMigrationContext.h"
#include "SatisfactorySave/GameTypes/FactoryGame/Online/ClientIdentification.h"
#include "libsavepy_common.h"

void init_GameTypes_FactoryGame(py::module_& m) {
    py::class_<s::FRailroadTrackPosition>(m, "FRailroadTrackPosition")
        .def(py::init<>())
        .def_readwrite("Track", &s::FRailroadTrackPosition::Track)
        .def_readwrite("Offset", &s::FRailroadTrackPosition::Offset)
        .def_readwrite("Forward", &s::FRailroadTrackPosition::Forward);

    py::class_<s::FClientIdentityInfo>(m, "FClientIdentityInfo")
        .def(py::init<>())
        .def_readwrite("OfflineId", &s::FClientIdentityInfo::OfflineId)
        .def_readwrite("AccountIds", &s::FClientIdentityInfo::AccountIds);

    py::class_<s::FObjectBaseSaveHeader>(m, "FObjectBaseSaveHeader")
        .def(py::init<>())
        .def_readwrite("ClassName", &s::FObjectBaseSaveHeader::ClassName)
        .def_readwrite("Reference", &s::FObjectBaseSaveHeader::Reference)
        .def_readwrite("ObjectFlags", &s::FObjectBaseSaveHeader::ObjectFlags);

    py::class_<s::FObjectSaveHeader>(m, "FObjectSaveHeader")
        .def(py::init<>())
        .def_readwrite("BaseHeader", &s::FObjectSaveHeader::BaseHeader)
        .def_readwrite("OuterPathName", &s::FObjectSaveHeader::OuterPathName);

    py::class_<s::FActorSaveHeader>(m, "FActorSaveHeader")
        .def(py::init<>())
        .def_readwrite("ObjectHeader", &s::FActorSaveHeader::ObjectHeader)
        .def_readwrite("Transform", &s::FActorSaveHeader::Transform)
        .def_readwrite("NeedTransform", &s::FActorSaveHeader::NeedTransform)
        .def_readwrite("WasPlacedInLevel", &s::FActorSaveHeader::WasPlacedInLevel);

    py::class_<s::FConveyorChainSplineSegment>(m, "FConveyorChainSplineSegment")
        .def(py::init<>())
        .def_readwrite("ChainActor", &s::FConveyorChainSplineSegment::ChainActor)
        .def_readwrite("ConveyorBase", &s::FConveyorChainSplineSegment::ConveyorBase)
        .def_readwrite("SplinePointData", &s::FConveyorChainSplineSegment::SplinePointData)
        .def_readwrite("OffsetAtStart", &s::FConveyorChainSplineSegment::OffsetAtStart)
        .def_readwrite("StartsAtLength", &s::FConveyorChainSplineSegment::StartsAtLength)
        .def_readwrite("EndsAtLength", &s::FConveyorChainSplineSegment::EndsAtLength)
        .def_readwrite("FirstItemIndex", &s::FConveyorChainSplineSegment::FirstItemIndex)
        .def_readwrite("LastItemIndex", &s::FConveyorChainSplineSegment::LastItemIndex)
        .def_readwrite("IndexInChainArray", &s::FConveyorChainSplineSegment::IndexInChainArray);

    py::class_<s::FConveyorBeltItem>(m, "FConveyorBeltItem")
        .def(py::init<>())
        .def_readwrite("Item", &s::FConveyorBeltItem::Item)
        .def_readwrite("Offset", &s::FConveyorBeltItem::Offset);

    py::class_<s::FConveyorBeltItems>(m, "FConveyorBeltItems")
        .def(py::init<>())
        .def_readwrite("Item", &s::FConveyorBeltItems::Items);

    py::class_<s::FDroneAction>(m, "FDroneAction")
        .def(py::init<>())
        .def_readwrite("actionStructName", &s::FDroneAction::actionStructName)
        .def_readwrite("action", &s::FDroneAction::action);

    py::class_<s::FFGDynamicStruct>(m, "FFGDynamicStruct")
        .def(py::init<>())
        .def_readwrite("ScriptStruct", &s::FFGDynamicStruct::ScriptStruct)
        .def_readwrite("StructInstance", &s::FFGDynamicStruct::StructInstance);

    py::class_<s::FBlueprintRecord>(m, "FBlueprintRecord")
        .def(py::init<>())
        .def_readwrite("ConfigVersion", &s::FBlueprintRecord::ConfigVersion)
        .def_readwrite("BlueprintDescription", &s::FBlueprintRecord::BlueprintDescription)
        .def_readwrite("IconID", &s::FBlueprintRecord::IconID)
        .def_readwrite("Color", &s::FBlueprintRecord::Color)
        .def_readwrite("LastEditedBy", &s::FBlueprintRecord::LastEditedBy);

    py::class_<s::FBlueprintItemAmount>(m, "FBlueprintItemAmount")
        .def(py::init<>())
        .def_readwrite("ItemClass", &s::FBlueprintItemAmount::ItemClass)
        .def_readwrite("Amount", &s::FBlueprintItemAmount::Amount);

    py::class_<s::FBlueprintHeader>(m, "FBlueprintHeader")
        .def(py::init<>())
        .def_readwrite("HeaderVersion", &s::FBlueprintHeader::HeaderVersion)
        .def_readwrite("SaveVersion", &s::FBlueprintHeader::SaveVersion)
        .def_readwrite("BuildVersion", &s::FBlueprintHeader::BuildVersion)
        .def_readwrite("Dimensions", &s::FBlueprintHeader::Dimensions)
        .def_readwrite("Cost", &s::FBlueprintHeader::Cost)
        .def_readwrite("RecipeRefs", &s::FBlueprintHeader::RecipeRefs);

    py::class_<s::FFactoryCustomizationColorSlot>(m, "FFactoryCustomizationColorSlot")
        .def(py::init<>())
        .def_readwrite("PrimaryColor", &s::FFactoryCustomizationColorSlot::PrimaryColor)
        .def_readwrite("SecondaryColor", &s::FFactoryCustomizationColorSlot::SecondaryColor)
        .def_readwrite("PaintFinish", &s::FFactoryCustomizationColorSlot::PaintFinish);

    py::class_<s::FFactoryCustomizationData>(m, "FFactoryCustomizationData")
        .def(py::init<>())
        .def_readwrite("SwatchDesc", &s::FFactoryCustomizationData::SwatchDesc)
        .def_readwrite("PatternDesc", &s::FFactoryCustomizationData::PatternDesc)
        .def_readwrite("MaterialDesc", &s::FFactoryCustomizationData::MaterialDesc)
        .def_readwrite("SkinDesc", &s::FFactoryCustomizationData::SkinDesc)
        .def_readwrite("OverrideColorData", &s::FFactoryCustomizationData::OverrideColorData)
        .def_readwrite("PatternRotation", &s::FFactoryCustomizationData::PatternRotation);

    py::class_<s::FFluidBox>(m, "FFluidBox")
        .def(py::init<>())
        .def_readwrite("Value", &s::FFluidBox::Value);

    py::class_<s::FPersistentGlobalIconId>(m, "FPersistentGlobalIconId")
        .def(py::init<>())
        .def_readwrite("IconLibrary", &s::FPersistentGlobalIconId::IconLibrary)
        .def_readwrite("IconID", &s::FPersistentGlobalIconId::IconID);

    py::class_<s::FInventoryItem>(m, "FInventoryItem")
        .def(py::init<>())
        .def_readwrite("ItemClass", &s::FInventoryItem::ItemClass)
        .def_readwrite("ItemState", &s::FInventoryItem::ItemState)
        .def_readwrite("LegacyItemStateActor", &s::FInventoryItem::LegacyItemStateActor);

    py::class_<s::FRuntimeBuildableInstanceData>(m, "FRuntimeBuildableInstanceData")
        .def(py::init<>())
        .def_readwrite("Transform", &s::FRuntimeBuildableInstanceData::Transform)
        .def_readwrite("CustomizationData", &s::FRuntimeBuildableInstanceData::CustomizationData)
        .def_readwrite("BuiltWithRecipe", &s::FRuntimeBuildableInstanceData::BuiltWithRecipe)
        .def_readwrite("BlueprintProxy", &s::FRuntimeBuildableInstanceData::BlueprintProxy)
        .def_readwrite("TypeSpecificData", &s::FRuntimeBuildableInstanceData::TypeSpecificData);

    py::class_<s::FObjectReferenceDisc>(m, "FObjectReferenceDisc")
        .def(py::init<>())
        .def(py::init<>([](std::string levelName, std::string pathName) {
            return s::FObjectReferenceDisc(std::move(levelName), std::move(pathName));
        }))
        .def_readwrite("LevelName", &s::FObjectReferenceDisc::LevelName)
        .def_readwrite("PathName", &s::FObjectReferenceDisc::PathName);

    py::class_<s::FSaveHeader>(m, "FSaveHeader")
        .def(py::init<>())
        .def_readwrite("SaveHeaderVersion", &s::FSaveHeader::SaveHeaderVersion)
        .def_readwrite("SaveVersion", &s::FSaveHeader::SaveVersion)
        .def_readwrite("BuildVersion", &s::FSaveHeader::BuildVersion)
        .def_readwrite("SaveName", &s::FSaveHeader::SaveName)
        .def_readwrite("MapName", &s::FSaveHeader::MapName)
        .def_readwrite("MapOptions", &s::FSaveHeader::MapOptions)
        .def_readwrite("SessionName", &s::FSaveHeader::SessionName)
        .def_readwrite("PlayDurationSeconds", &s::FSaveHeader::PlayDurationSeconds)
        .def_readwrite("SaveDateTime", &s::FSaveHeader::SaveDateTime)
        .def_readwrite("SessionVisibility", &s::FSaveHeader::SessionVisibility)
        .def_readwrite("EditorObjectVersion", &s::FSaveHeader::EditorObjectVersion)
        .def_readwrite("ModMetadata", &s::FSaveHeader::ModMetadata)
        .def_readwrite("IsModdedSave", &s::FSaveHeader::IsModdedSave)
        .def_readwrite("SaveIdentifier", &s::FSaveHeader::SaveIdentifier)
        .def_readwrite("IsPartitionedWorld", &s::FSaveHeader::IsPartitionedWorld)
        .def_readwrite("SaveDataHash", &s::FSaveHeader::SaveDataHash)
        .def_readwrite("IsCreativeModeEnabled", &s::FSaveHeader::IsCreativeModeEnabled)
        .def("toString", &s::FSaveHeader::toString);

    py::class_<s::FPerStreamingLevelSaveData>(m, "FPerStreamingLevelSaveData")
        .def(py::init<>())
        .def_readwrite("SaveObjects", &s::FPerStreamingLevelSaveData::SaveObjects)
        .def_readwrite("TOC_DestroyedActors", &s::FPerStreamingLevelSaveData::TOC_DestroyedActors)
        .def_readwrite("SaveVersion", &s::FPerStreamingLevelSaveData::SaveVersion)
        .def_readwrite("DestroyedActors", &s::FPerStreamingLevelSaveData::DestroyedActors);

    py::class_<s::FPersistentAndRuntimeSaveData>(m, "FPersistentAndRuntimeSaveData")
        .def(py::init<>())
        .def_readwrite("SaveObjects", &s::FPersistentAndRuntimeSaveData::SaveObjects)
        .def_readwrite("TOC_LevelToDestroyedActorsMap", &s::FPersistentAndRuntimeSaveData::TOC_LevelToDestroyedActorsMap)
        .def_readwrite("LevelToDestroyedActorsMap", &s::FPersistentAndRuntimeSaveData::LevelToDestroyedActorsMap);

    py::class_<s::FUnresolvedWorldSaveData>(m, "FUnresolvedWorldSaveData")
        .def(py::init<>())
        .def_readwrite("DestroyedActors", &s::FUnresolvedWorldSaveData::DestroyedActors);

    py::class_<s::FVehiclePhysicsData>(m, "FVehiclePhysicsData")
        .def(py::init<>())
        .def_readwrite("BoneName", &s::FVehiclePhysicsData::BoneName)
        .def_readwrite("BodyState", &s::FVehiclePhysicsData::BodyState);

    py::class_<s::FWPGridValidationData>(m, "FWPGridValidationData")
        .def(py::init<>())
        .def_readwrite("CellSize", &s::FWPGridValidationData::CellSize)
        .def_readwrite("GridHash", &s::FWPGridValidationData::GridHash)
        .def_readwrite("CellHashes", &s::FWPGridValidationData::CellHashes);

    py::class_<s::FWorldPartitionValidationData>(m, "FWorldPartitionValidationData")
        .def(py::init<>())
        .def_readwrite("Grids", &s::FWorldPartitionValidationData::Grids);
}
