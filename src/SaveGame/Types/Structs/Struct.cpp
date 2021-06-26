#include "Struct.h"

#include "BoxStruct.h"
#include "ColorStruct.h"
#include "FluidBoxStruct.h"
#include "InventoryItemStruct.h"
#include "LinearColorStruct.h"
#include "PropertyStruct.h"
#include "QuatStruct.h"
#include "RailroadTrackPositionStruct.h"
#include "VectorStruct.h"

std::unique_ptr<Satisfactory3DMap::Struct> Satisfactory3DMap::Struct::parse(
    const std::string& struct_name, std::istream& stream) {

    if (struct_name == "Box") {
        return std::make_unique<BoxStruct>(struct_name, stream);
    } else if (struct_name == "Color") {
        return std::make_unique<ColorStruct>(struct_name, stream);
    } else if (struct_name == "FluidBox") {
        return std::make_unique<FluidBoxStruct>(struct_name, stream);
    } else if (struct_name == "InventoryItem") {
        return std::make_unique<InventoryItemStruct>(struct_name, stream);
    } else if (struct_name == "LinearColor") {
        return std::make_unique<LinearColorStruct>(struct_name, stream);
    } else if (struct_name == "Quat") {
        return std::make_unique<QuatStruct>(struct_name, stream);
    } else if (struct_name == "RailroadTrackPosition") {
        return std::make_unique<RailroadTrackPositionStruct>(struct_name, stream);
    } else if (struct_name == "Vector") {
        return std::make_unique<VectorStruct>(struct_name, stream);
    } else if (struct_name == "DroneDockingStateInfo" || struct_name == "DroneTripInformation" ||
               struct_name == "FeetOffset" || struct_name == "Hotbar" || struct_name == "InventoryStack" ||
               struct_name == "ItemAmount" || struct_name == "ItemFoundData" ||
               struct_name == "LightSourceControlData" || struct_name == "MessageData" || struct_name == "PhaseCost" ||
               struct_name == "RemovedInstance" || struct_name == "RemovedInstanceArray" ||
               struct_name == "ResearchData" || struct_name == "ResearchTime" ||
               struct_name == "ScannableResourcePair" || struct_name == "SchematicCost" || struct_name == "SpawnData" ||
               struct_name == "SplinePointData" || struct_name == "SplitterSortRule" || struct_name == "TimerHandle" ||
               struct_name == "TimeTableStop" || struct_name == "TrainSimulationData" || struct_name == "Transform") {
        return std::make_unique<PropertyStruct>(struct_name, stream);
    } else {
        throw std::runtime_error("Struct name \"" + struct_name + "\" not implemented!");
    }
}
