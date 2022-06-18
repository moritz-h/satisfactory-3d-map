#include "Struct.h"

#include "BoxStruct.h"
#include "ColorStruct.h"
#include "FluidBoxStruct.h"
#include "GuidStruct.h"
#include "IntPointStruct.h"
#include "InventoryItemStruct.h"
#include "LinearColorStruct.h"
#include "MaterialInput.h"
#include "PropertyStruct.h"
#include "QuatStruct.h"
#include "RailroadTrackPositionStruct.h"
#include "RotatorStruct.h"
#include "VectorStruct.h"

std::unique_ptr<Satisfactory3DMap::Struct> Satisfactory3DMap::Struct::create(const FName& struct_name, Archive& ar) {

    std::unique_ptr<Struct> s;

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
    } else if (struct_name == "InventoryItem") {
        s = std::make_unique<InventoryItemStruct>(struct_name);
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
    } else if (struct_name == "VectorMaterialInput") {
        s = std::make_unique<VectorMaterialInputStruct>(struct_name);
    } else if (struct_name == "Vector") {
        s = std::make_unique<VectorStruct>(struct_name);
    } else if (struct_name == "BodyInstance" || struct_name == "BoxSphereBounds" ||
               struct_name == "CollisionResponse" || struct_name == "DroneDockingStateInfo" ||
               struct_name == "DroneTripInformation" || struct_name == "FactoryCustomizationColorSlot" ||
               struct_name == "FactoryCustomizationData" || struct_name == "FeetOffset" ||
               struct_name == "FoundationSideSelectionFlags" || struct_name == "Hotbar" ||
               struct_name == "InventoryStack" || struct_name == "ItemAmount" || struct_name == "ItemFoundData" ||
               struct_name == "LightSourceControlData" || struct_name == "MapMarker" ||
               struct_name == "MaterialCachedExpressionData" ||
               struct_name == "MaterialInstanceBasePropertyOverrides" || struct_name == "MaterialParameterInfo" ||
               struct_name == "MeshUVChannelInfo" || struct_name == "MessageData" || struct_name == "MiniGameResult" ||
               struct_name == "PhaseCost" || struct_name == "PointerToUberGraphFrame" ||
               struct_name == "PrefabIconElementSaveData" || struct_name == "PrefabTextElementSaveData" ||
               struct_name == "RecipeAmountStruct" || struct_name == "RemovedInstance" ||
               struct_name == "RemovedInstanceArray" || struct_name == "ResearchData" ||
               struct_name == "ResearchTime" || struct_name == "ResponseChannel" ||
               struct_name == "ScalarParameterValue" || struct_name == "ScannableResourcePair" ||
               struct_name == "SchematicCost" || struct_name == "SpawnData" || struct_name == "SplinePointData" ||
               struct_name == "SplitterSortRule" || struct_name == "StaticMaterial" ||
               struct_name == "StaticParameterSet" || struct_name == "StaticSwitchParameter" ||
               struct_name == "SubCategoryMaterialDefault" || struct_name == "TextureParameterValue" ||
               struct_name == "TimerHandle" || struct_name == "TimeTableStop" || struct_name == "TrainDockingRuleSet" ||
               struct_name == "TrainSimulationData" || struct_name == "Transform" ||
               struct_name == "Vector_NetQuantize") {
        s = std::make_unique<PropertyStruct>(struct_name);
    } else {
        throw std::runtime_error("Struct name \"" + struct_name + "\" not implemented!");
    }

    ar << *s;

    return s;
}
