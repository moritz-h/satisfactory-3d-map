#include "Struct.h"

#include <unordered_set>

#include <spdlog/spdlog.h>

#include "BoxStruct.h"
#include "ColorStruct.h"
#include "FluidBoxStruct.h"
#include "GuidStruct.h"
#include "IntPointStruct.h"
#include "IntVectorStruct.h"
#include "InventoryItemStruct.h"
#include "LinearColorStruct.h"
#include "MaterialInput.h"
#include "Mods/LBBalancerIndexingStruct.h"
#include "PropertyStruct.h"
#include "QuatStruct.h"
#include "RailroadTrackPositionStruct.h"
#include "RotatorStruct.h"
#include "SoftClassPathStruct.h"
#include "Vector2DStruct.h"
#include "VectorStruct.h"

std::unique_ptr<Satisfactory3DMap::Struct> Satisfactory3DMap::Struct::create(const FName& struct_name, Archive& ar) {

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
    };

    if (struct_name == "Box") {
        s = std::make_unique<BoxStruct>(struct_name);
    } else if (struct_name == "Color") {
        s = std::make_unique<ColorStruct>(struct_name);
    } else if (struct_name == "FluidBox") {
        s = std::make_unique<FluidBoxStruct>(struct_name);
    } else if (struct_name == "Guid") {
        s = std::make_unique<GuidStruct>(struct_name);
    } else if (struct_name == "IntPoint") {
        s = std::make_unique<IntPointStruct>(struct_name);
    } else if (struct_name == "IntVector") {
        s = std::make_unique<IntVectorStruct>(struct_name);
    } else if (struct_name == "InventoryItem") {
        s = std::make_unique<InventoryItemStruct>(struct_name);
    } else if (struct_name == "LBBalancerIndexing") {
        s = std::make_unique<LBBalancerIndexingStruct>(struct_name);
    } else if (struct_name == "LinearColor") {
        s = std::make_unique<LinearColorStruct>(struct_name);
    } else if (struct_name == "Quat") {
        s = std::make_unique<QuatStruct>(struct_name);
    } else if (struct_name == "RailroadTrackPosition") {
        s = std::make_unique<RailroadTrackPositionStruct>(struct_name);
    } else if (struct_name == "Rotator") {
        s = std::make_unique<RotatorStruct>(struct_name);
    } else if (struct_name == "ScalarMaterialInput") {
        s = std::make_unique<ScalarMaterialInputStruct>(struct_name);
    } else if (struct_name == "SoftClassPath") {
        s = std::make_unique<SoftClassPathStruct>(struct_name);
    } else if (struct_name == "VectorMaterialInput") {
        s = std::make_unique<VectorMaterialInputStruct>(struct_name);
    } else if (struct_name == "Vector2D") {
        s = std::make_unique<Vector2DStruct>(struct_name);
    } else if (struct_name == "Vector") {
        s = std::make_unique<VectorStruct>(struct_name);
    } else if (propertyStructNames.count(struct_name.toString()) > 0) {
        s = std::make_unique<PropertyStruct>(struct_name);
    } else {
        spdlog::warn("Unknown struct name \"{}\", try parsing as PropertyStruct.", struct_name.toString());
        s = std::make_unique<PropertyStruct>(struct_name);
    }

    ar << *s;

    return s;
}
