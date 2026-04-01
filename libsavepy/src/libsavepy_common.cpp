#include "libsavepy_common.h"

#include <pybind11/stl_bind.h>

#include "SatisfactorySave/GameTypes/FactoryGame/FGSaveSession.h"

void init_vector(py::module_& m) {
    py::bind_vector<std::vector<int8_t>, py::smart_holder>(m, "StdVectorInt8");
    py::bind_vector<std::vector<int16_t>, py::smart_holder>(m, "StdVectorInt16");
    py::bind_vector<std::vector<int32_t>, py::smart_holder>(m, "StdVectorInt32");
    py::bind_vector<std::vector<int64_t>, py::smart_holder>(m, "StdVectorInt64");
    py::bind_vector<std::vector<uint8_t>, py::smart_holder>(m, "StdVectorUInt8");
    py::bind_vector<std::vector<uint16_t>, py::smart_holder>(m, "StdVectorUInt16");
    py::bind_vector<std::vector<uint32_t>, py::smart_holder>(m, "StdVectorUInt32");
    py::bind_vector<std::vector<uint64_t>, py::smart_holder>(m, "StdVectorUInt64");
    py::bind_vector<std::vector<float>, py::smart_holder>(m, "StdVectorFloat");
    py::bind_vector<std::vector<double>, py::smart_holder>(m, "StdVectorDouble");
    py::bind_vector<std::vector<std::string>, py::smart_holder>(m, "StdVectorString");
    py::bind_vector<std::vector<s::FCustomVersion>, py::smart_holder>(m, "StdVectorFCustomVersion");
    py::bind_vector<std::vector<s::FName>, py::smart_holder>(m, "StdVectorFName");
    py::bind_vector<std::vector<s::FObjectReferenceDisc>, py::smart_holder>(m, "StdVectorFObjectReferenceDisc");
    py::bind_vector<std::vector<s::FSoftObjectPath>, py::smart_holder>(m, "StdVectorFSoftObjectPath");
    py::bind_vector<std::vector<s::ScriptDelegate>, py::smart_holder>(m, "StdVectorScriptDelegate");
    py::bind_vector<std::vector<s::FConveyorBeltItem>, py::smart_holder>(m, "StdVectorFConveyorBeltItem");
    py::bind_vector<std::vector<s::FConveyorChainSplineSegment>, py::smart_holder>(m, "StdVectorFConveyorChainSplineSegment");
    py::bind_vector<std::vector<s::FDroneAction>, py::smart_holder>(m, "StdVectorFDroneAction");
    py::bind_vector<std::vector<s::FRuntimeBuildableInstanceData>, py::smart_holder>(m, "StdVectorFRuntimeBuildableInstanceData");
    py::bind_vector<std::vector<s::FSplinePointData>, py::smart_holder>(m, "StdVectorFSplinePointData");
    py::bind_vector<std::vector<s::FVehiclePhysicsData>, py::smart_holder>(m, "StdVectorFVehiclePhysicsData");
    py::bind_vector<s::SaveObjectList, py::smart_holder>(m, "SaveObjectList");
    py::bind_vector<std::vector<std::shared_ptr<s::Struct>>, py::smart_holder>(m, "StdVectorStruct");
    py::bind_vector<std::vector<s::FLocalUserNetIdBundle>, py::smart_holder>(m, "StdVectorFLocalUserNetIdBundle");

    py::implicitly_convertible<py::list, std::vector<int8_t>>();
    py::implicitly_convertible<py::list, std::vector<int16_t>>();
    py::implicitly_convertible<py::list, std::vector<int32_t>>();
    py::implicitly_convertible<py::list, std::vector<int64_t>>();
    py::implicitly_convertible<py::list, std::vector<uint8_t>>();
    py::implicitly_convertible<py::list, std::vector<uint16_t>>();
    py::implicitly_convertible<py::list, std::vector<uint32_t>>();
    py::implicitly_convertible<py::list, std::vector<uint64_t>>();
    py::implicitly_convertible<py::list, std::vector<float>>();
    py::implicitly_convertible<py::list, std::vector<double>>();
    py::implicitly_convertible<py::list, std::vector<std::string>>();
    py::implicitly_convertible<py::list, std::vector<s::FCustomVersion>>();
    py::implicitly_convertible<py::list, std::vector<s::FName>>();
    py::implicitly_convertible<py::list, std::vector<s::FObjectReferenceDisc>>();
    py::implicitly_convertible<py::list, std::vector<s::FSoftObjectPath>>();
    py::implicitly_convertible<py::list, std::vector<s::ScriptDelegate>>();
    py::implicitly_convertible<py::list, std::vector<s::FConveyorBeltItem>>();
    py::implicitly_convertible<py::list, std::vector<s::FConveyorChainSplineSegment>>();
    py::implicitly_convertible<py::list, std::vector<s::FDroneAction>>();
    py::implicitly_convertible<py::list, std::vector<s::FRuntimeBuildableInstanceData>>();
    py::implicitly_convertible<py::list, std::vector<s::FSplinePointData>>();
    py::implicitly_convertible<py::list, std::vector<s::FVehiclePhysicsData>>();
    py::implicitly_convertible<py::list, s::SaveObjectList>();
    py::implicitly_convertible<py::list, std::vector<std::shared_ptr<s::Struct>>>();
    py::implicitly_convertible<py::list, std::vector<s::FLocalUserNetIdBundle>>();
}

template<typename K, typename V>
inline void init_tmap_type(py::module& m, const char* name) {
    py::classh<s::TMap<K, V>>(m, name)
        .def_readwrite("Keys", &s::TMap<K, V>::Keys)
        .def_readwrite("Values", &s::TMap<K, V>::Values)
        .def("__len__", [](const s::TMap<K, V>& map) { return map.size(); })
        .def("__getitem__", &s::TMap<K, V>::operator[], py::return_value_policy::reference_internal)
        .def("__setitem__", [](s::TMap<K, V>& map, K& k, V& v) { map[k] = v; });
}

void init_tmap(py::module_& m) {
    // FClientIdentityInfo::AccountIds
    init_tmap_type<uint8_t, std::vector<uint8_t>>(m, "Map<uint8,vector<uint8>>");

    // AFGCircuitSubsystem::mCircuits
    init_tmap_type<uint32_t, s::FObjectReferenceDisc>(m, "Map<uint32,FObjectReferenceDisc>");

    // AFGLightweightBuildableSubsystem::mBuildableClassToInstanceArray
    init_tmap_type<s::FObjectReferenceDisc, std::vector<s::FRuntimeBuildableInstanceData>>(m, "Map<FObjectReferenceDisc,vector<FRuntimeBuildableInstanceData>>");

    // FWPGridValidationData::CellHashes
    init_tmap_type<s::FName, uint32_t>(m, "Map<FName,uint32>");

    // FWorldPartitionValidationData::Grids
    init_tmap_type<s::FName, s::FWPGridValidationData>(m, "Map<FName,FWPGridValidationData>");

    // FPersistentAndRuntimeSaveData::TOC_LevelToDestroyedActorsMap
    // FPersistentAndRuntimeSaveData::LevelToDestroyedActorsMap
    init_tmap_type<std::string, std::vector<s::FObjectReferenceDisc>>(m, "Map<String,vector<FObjectReferenceDisc>>");

    // SaveGame::mPerLevelDataMap
    init_tmap_type<std::string, s::FPerStreamingLevelSaveData>(m, "Map<String,FPerStreamingLevelSaveData>");
}
