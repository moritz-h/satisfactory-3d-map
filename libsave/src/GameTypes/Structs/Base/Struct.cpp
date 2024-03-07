#include "GameTypes/Structs/Base/Struct.h"

#include <unordered_set>

#include <spdlog/spdlog.h>

#include "GameTypes/Structs/Base/StructAll.h"

std::unique_ptr<SatisfactorySave::Struct> SatisfactorySave::Struct::create(const FName& struct_name, Archive& ar) {

    std::unique_ptr<Struct> s;

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
        "DroneDockingStateInfo",
        "DroneTripInformation",
        "FactoryCustomizationColorSlot",
        "FactoryCustomizationData",
        "FactoryTickFunction",
        "FeetOffset",
        "FloatInterval",
        "FloatRange",
        "FloatRangeBound",
        "FoliageRemovalSaveDataForFoliageType",
        "FoliageRemovalSaveDataPerCell",
        "FoliageRemovalUnresolvedSaveDataPerCell",
        "FoundationSideSelectionFlags",
        "HeadlightParams",
        "Hotbar",
        "InstanceData",
        "InventoryStack",
        "ItemAmount",
        "ItemFoundData",
        "LBBalancerData", // Mod LoadBalancers
        "LightSourceControlData",
        "MapMarker",
        "MaterialCachedExpressionData",
        "MaterialInstanceBasePropertyOverrides",
        "MaterialParameterInfo",
        "MeshUVChannelInfo",
        "MessageData",
        "MiniGameResult",
        "ParticleMap",
        "PhaseCost",
        "PlayerRules",
        "PointerToUberGraphFrame",
        "PrefabIconElementSaveData",
        "PrefabTextElementSaveData",
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
        "SpawnData",
        "SplinePointData",
        "SplitterSortRule",
        "StaticMaterial",
        "StaticParameterSet",
        "StaticSwitchParameter",
        "StringPair",
        "SubCategoryMaterialDefault",
        "TextureParameterValue",
        "TimerHandle",
        "TimeTableStop",
        "TireTrackDecalDetails",
        "TrainDockingRuleSet",
        "TrainSimulationData",
        "Transform",
        "Vector_NetQuantize",
        "WireInstance",
    };

    if (struct_name == BoxStruct::TypeName) {
        s = std::make_unique<BoxStruct>();
    } else if (struct_name == ColorStruct::TypeName) {
        s = std::make_unique<ColorStruct>();
    } else if (struct_name == FluidBoxStruct::TypeName) {
        s = std::make_unique<FluidBoxStruct>();
    } else if (struct_name == GuidStruct::TypeName) {
        s = std::make_unique<GuidStruct>();
    } else if (struct_name == IntPointStruct::TypeName) {
        s = std::make_unique<IntPointStruct>();
    } else if (struct_name == IntVectorStruct::TypeName) {
        s = std::make_unique<IntVectorStruct>();
    } else if (struct_name == InventoryItemStruct::TypeName) {
        s = std::make_unique<InventoryItemStruct>();
    } else if (struct_name == LinearColorStruct::TypeName) {
        s = std::make_unique<LinearColorStruct>();
    } else if (struct_name == QuatStruct::TypeName) {
        s = std::make_unique<QuatStruct>();
    } else if (struct_name == RailroadTrackPositionStruct::TypeName) {
        s = std::make_unique<RailroadTrackPositionStruct>();
    } else if (struct_name == RotatorStruct::TypeName) {
        s = std::make_unique<RotatorStruct>();
    } else if (struct_name == ScalarMaterialInputStruct::TypeName) {
        s = std::make_unique<ScalarMaterialInputStruct>();
    } else if (struct_name == SoftClassPathStruct::TypeName) {
        s = std::make_unique<SoftClassPathStruct>();
    } else if (struct_name == Vector2DStruct::TypeName) {
        s = std::make_unique<Vector2DStruct>();
    } else if (struct_name == VectorMaterialInputStruct::TypeName) {
        s = std::make_unique<VectorMaterialInputStruct>();
    } else if (struct_name == VectorStruct::TypeName) {
        s = std::make_unique<VectorStruct>();
    } else if (struct_name == LBBalancerIndexingStruct::TypeName) {
        s = std::make_unique<LBBalancerIndexingStruct>();
    } else if (propertyStructNames.contains(struct_name.toString())) {
        s = std::make_unique<PropertyStruct>(struct_name);
    } else {
        spdlog::warn("Unknown struct name \"{}\", try parsing as PropertyStruct.", struct_name.toString());
        s = std::make_unique<PropertyStruct>(struct_name);
    }

    ar << *s;

    return s;
}
