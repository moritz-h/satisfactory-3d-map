#include <pybind11/pybind11.h>
#include <pybind11/stl_bind.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/spdlog.h>

#include "SatisfactorySave/GameTypes/Save/SaveGame.h"
#include "libsavepy_common.h"

namespace py = pybind11;
namespace s = SatisfactorySave;

void init_common(py::module_&);
void init_tmap(py::module_&);
void init_GameTypes_Arrays(py::module_&);
void init_GameTypes_MapTypes(py::module_&);
void init_GameTypes_Properties(py::module_&);
void init_GameTypes_Save(py::module_&);
void init_GameTypes_Sets(py::module_&);
void init_GameTypes_Structs(py::module_&);
void init_GameTypes_UE_Engine(py::module_&);
void init_GameTypes_UE_GameFramework(py::module_&);
void init_GameTypes_UE_Internationalization(py::module_&);
void init_GameTypes_UE_Math(py::module_&);
void init_GameTypes_UE_Misc(py::module_&);
void init_GameTypes_UE_Satisfactory(py::module_&);
void init_GameTypes_UE_SatisfactoryObjects(py::module_&);
void init_GameTypes_UE_UObject(py::module_&);

PYBIND11_MODULE(satisfactory_save, m) {
    // Register a spdlog default logger
    auto cout_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
    cout_sink->set_pattern("[%T.%e] %^[%L] %v%$");
    auto logger = std::make_shared<spdlog::logger>("", std::initializer_list<spdlog::sink_ptr>{cout_sink});
    logger->set_level(spdlog::level::trace);
    logger->flush_on(spdlog::level::trace);
    spdlog::set_default_logger(logger);

    init_common(m);
    init_tmap(m);

    init_GameTypes_UE_UObject(m);
    init_GameTypes_UE_Engine(m);
    init_GameTypes_UE_GameFramework(m);
    init_GameTypes_UE_Internationalization(m);
    init_GameTypes_UE_Math(m);
    init_GameTypes_UE_Misc(m);
    init_GameTypes_UE_Satisfactory(m);
    init_GameTypes_UE_SatisfactoryObjects(m);

    init_GameTypes_Arrays(m);
    init_GameTypes_MapTypes(m);
    init_GameTypes_Sets(m);
    init_GameTypes_Structs(m);

    init_GameTypes_Properties(m);

    init_GameTypes_Save(m);
}

void init_common(py::module_& m) {
    py::bind_vector<std::vector<int8_t>>(m, "StdVectorInt8");
    py::bind_vector<std::vector<int16_t>>(m, "StdVectorInt16");
    py::bind_vector<std::vector<int32_t>>(m, "StdVectorInt32");
    py::bind_vector<std::vector<int64_t>>(m, "StdVectorInt64");
    py::bind_vector<std::vector<uint8_t>>(m, "StdVectorUInt8");
    py::bind_vector<std::vector<uint16_t>>(m, "StdVectorUInt16");
    py::bind_vector<std::vector<uint32_t>>(m, "StdVectorUInt32");
    py::bind_vector<std::vector<uint64_t>>(m, "StdVectorUInt64");
    py::bind_vector<std::vector<float>>(m, "StdVectorFloat");
    py::bind_vector<std::vector<double>>(m, "StdVectorDouble");
    py::bind_vector<std::vector<std::string>>(m, "StdVectorString");
    py::bind_vector<std::vector<s::FName>>(m, "StdVectorFName");
    py::bind_vector<std::vector<s::FObjectReferenceDisc>>(m, "StdVectorFObjectReferenceDisc");
    py::bind_vector<std::vector<s::FSoftObjectPath>>(m, "StdVectorFSoftObjectPath");
    py::bind_vector<std::vector<s::FConveyorBeltItem>>(m, "StdVectorFConveyorBeltItem");
    py::bind_vector<std::vector<s::FConveyorChainSplineSegment>>(m, "StdVectorFConveyorChainSplineSegment");
    py::bind_vector<std::vector<s::FDroneAction>>(m, "StdVectorFDroneAction");
    py::bind_vector<std::vector<s::FRuntimeBuildableInstanceData>>(m, "StdVectorFRuntimeBuildableInstanceData");
    py::bind_vector<std::vector<s::FSplinePointData>>(m, "StdVectorFSplinePointData");
    py::bind_vector<std::vector<s::FVehiclePhysicsData>>(m, "StdVectorFVehiclePhysicsData");
    py::bind_vector<s::SaveObjectList>(m, "SaveObjectList");
    py::bind_vector<std::vector<std::shared_ptr<s::Struct>>>(m, "StdVectorStruct");

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
    py::implicitly_convertible<py::list, std::vector<s::FName>>();
    py::implicitly_convertible<py::list, std::vector<s::FObjectReferenceDisc>>();
    py::implicitly_convertible<py::list, std::vector<s::FSoftObjectPath>>();
    py::implicitly_convertible<py::list, std::vector<s::FConveyorBeltItem>>();
    py::implicitly_convertible<py::list, std::vector<s::FConveyorChainSplineSegment>>();
    py::implicitly_convertible<py::list, std::vector<s::FDroneAction>>();
    py::implicitly_convertible<py::list, std::vector<s::FRuntimeBuildableInstanceData>>();
    py::implicitly_convertible<py::list, std::vector<s::FSplinePointData>>();
    py::implicitly_convertible<py::list, std::vector<s::FVehiclePhysicsData>>();
    py::implicitly_convertible<py::list, s::SaveObjectList>();
    py::implicitly_convertible<py::list, std::vector<std::shared_ptr<s::Struct>>>();
}

template<typename K, typename V>
inline void init_tmap_type(py::module& m, const char* name) {
    py::class_<s::TMap<K, V>>(m, name)
        .def_readwrite("Keys", &s::TMap<K, V>::Keys)
        .def_readwrite("Values", &s::TMap<K, V>::Values)
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
}
