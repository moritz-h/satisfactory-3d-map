#include "GameTypes/Structs/Base/Struct.h"

#include <unordered_set>

#include <spdlog/spdlog.h>

#include "GameTypes/Structs/Base/StructAll.h"

std::shared_ptr<SatisfactorySave::Struct> SatisfactorySave::Struct::create(const FName& struct_name, Archive& ar) {

    std::shared_ptr<Struct> s;

    static const std::unordered_set<std::string> propertyStructNames{
        "ActorBuiltData",
        "ActorTickFunction",
        "BlueprintCategoryRecord",
        "BlueprintRecord",
        "BlueprintSubCategoryRecord",
        "BodyInstance",
        "BoomBoxPlayerState",
        "BoxSphereBounds",
        "CalendarData",
        "CollisionResponse",
        "CustomizationDescToRecipeData",
        "DockingVehicleStatistics",
        "DroneDockingStateInfo",
        "DroneTripInformation",
        "ElevatorFloorStopInfo",
        "FactoryCustomizationColorSlot",
        "FactoryCustomizationData",
        "FactoryTickFunction",
        "FeetOffset",
        "FGCachedConnectedWire",
        "FGDroneFuelRuntimeData",
        "FGPortalCachedFactoryTickData",
        "FloatInterval",
        "FloatRange",
        "FloatRangeBound",
        "FoliageRemovalSaveDataForFoliageType",
        "FoliageRemovalSaveDataPerCell",
        "FoliageRemovalUnresolvedSaveDataPerCell",
        "FoundationSideSelectionFlags",
        "GCheckmarkUnlockData",
        "GlobalColorPreset",
        "HardDriveData",
        "HeadlightParams",
        "HighlightedMarkerPair",
        "Hotbar",
        "InstanceData",
        "InventoryStack",
        "InventoryToRespawnWith",
        "ItemAmount",
        "ItemFoundData",
        "KAggregateGeom",
        "KConvexElem",
        "LBBalancerData", // Mod LoadBalancers
        "LightSourceControlData",
        "LocalUserNetIdBundle",
        "MapMarker",
        "MappedItemAmount",
        "MaterialCachedExpressionData",
        "MaterialInstanceBasePropertyOverrides",
        "MaterialParameterInfo",
        "MeshUVChannelInfo",
        "MessageData",
        "MiniGameResult",
        "ParticleMap",
        "PersistentGlobalIconId",
        "PhaseCost",
        "PlayerCustomizationData",
        "PlayerRules",
        "PlayerStateSetWrapper",
        "PointerToUberGraphFrame",
        "PrefabIconElementSaveData",
        "PrefabTextElementSaveData",
        "ProjectAssemblyLaunchSequenceValue",
        "RecipeAmountStruct",
        "RemovedInstance",
        "RemovedInstanceArray",
        "ResearchData",
        "ResearchTime",
        "ResourceSinkHistory",
        "ResponseChannel",
        "ScalarParameterValue",
        "ScannableObjectData",
        "ScannableResourcePair",
        "SchematicCost",
        "ShoppingListClassEntry",
        "ShoppingListSettings",
        "SpawnData",
        "SplinePointData",
        "SplitterSortRule",
        "StaticMaterial",
        "StaticParameterSet",
        "StaticSwitchParameter",
        "StreamingTextureBuildInfo",
        "StringPair",
        "SubCategoryMaterialDefault",
        "SwatchGroupData",
        "SwitchData",
        "TextureParameterValue",
        "TimerHandle",
        "TimeTableStop",
        "TireTrackDecalDetails",
        "TopLevelAssetPath",
        "TrainDockingRuleSet",
        "TrainSimulationData",
        "Transform",
        "Vector_NetQuantize",
        "WeightmapLayerAllocationInfo",
        "WireInstance",
        "WorldPartitionRuntimeCellDebugInfo",
    };

    if (struct_name == BoxStruct::TypeName) {
        s = std::make_shared<BoxStruct>();
    } else if (struct_name == ClientIdentityInfoStruct::TypeName) {
        s = std::make_shared<ClientIdentityInfoStruct>();
    } else if (struct_name == ColorStruct::TypeName) {
        s = std::make_shared<ColorStruct>();
    } else if (struct_name == DateTimeStruct::TypeName) {
        s = std::make_shared<DateTimeStruct>();
    } else if (struct_name == FluidBoxStruct::TypeName) {
        s = std::make_shared<FluidBoxStruct>();
    } else if (struct_name == GuidStruct::TypeName) {
        s = std::make_shared<GuidStruct>();
    } else if (struct_name == IntPointStruct::TypeName) {
        s = std::make_shared<IntPointStruct>();
    } else if (struct_name == IntVectorStruct::TypeName) {
        s = std::make_shared<IntVectorStruct>();
    } else if (struct_name == InventoryItemStruct::TypeName) {
        s = std::make_shared<InventoryItemStruct>();
    } else if (struct_name == LinearColorStruct::TypeName) {
        s = std::make_shared<LinearColorStruct>();
    } else if (struct_name == QuatStruct::TypeName) {
        s = std::make_shared<QuatStruct>();
    } else if (struct_name == RailroadTrackPositionStruct::TypeName) {
        s = std::make_shared<RailroadTrackPositionStruct>();
    } else if (struct_name == RotatorStruct::TypeName) {
        s = std::make_shared<RotatorStruct>();
    } else if (struct_name == ScalarMaterialInputStruct::TypeName) {
        s = std::make_shared<ScalarMaterialInputStruct>();
    } else if (struct_name == SoftClassPathStruct::TypeName) {
        s = std::make_shared<SoftClassPathStruct>();
    } else if (struct_name == Vector2DStruct::TypeName) {
        s = std::make_shared<Vector2DStruct>();
    } else if (struct_name == Vector4Struct::TypeName) {
        s = std::make_shared<Vector4Struct>();
    } else if (struct_name == VectorMaterialInputStruct::TypeName) {
        s = std::make_shared<VectorMaterialInputStruct>();
    } else if (struct_name == VectorStruct::TypeName) {
        s = std::make_shared<VectorStruct>();
    } else if (struct_name == LBBalancerIndexingStruct::TypeName) {
        s = std::make_shared<LBBalancerIndexingStruct>();
    } else if (propertyStructNames.contains(struct_name.toString())) {
        s = std::make_shared<PropertyStruct>(struct_name);
    } else {
        spdlog::warn("Unknown struct name \"{}\", try parsing as PropertyStruct.", struct_name.toString());
        s = std::make_shared<PropertyStruct>(struct_name);
    }

    ar << *s;

    return s;
}
