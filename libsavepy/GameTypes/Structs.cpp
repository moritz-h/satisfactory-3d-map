#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include "SatisfactorySave/GameTypes/Structs/Base/Struct.h"
#include "SatisfactorySave/GameTypes/Structs/Base/StructAll.h"

namespace py = pybind11;
namespace s = SatisfactorySave;

void init_GameTypes_Structs(py::module_& m) {
    py::class_<s::Struct>(m, "Struct")
        .def("name", &s::Struct::name);

    py::class_<s::BoxStruct, s::Struct>(m, "BoxStruct")
        .def_readwrite("Data", &s::BoxStruct::Data);

    py::class_<s::ColorStruct, s::Struct>(m, "ColorStruct")
        .def_readwrite("Data", &s::ColorStruct::Data);

    py::class_<s::FluidBoxStruct, s::Struct>(m, "FluidBoxStruct")
        .def_readwrite("Data", &s::FluidBoxStruct::Data);

    py::class_<s::GuidStruct, s::Struct>(m, "GuidStruct")
        .def_readwrite("Data", &s::GuidStruct::Data);

    py::class_<s::IntPointStruct, s::Struct>(m, "IntPointStruct")
        .def_readwrite("Data", &s::IntPointStruct::Data);

    py::class_<s::IntVectorStruct, s::Struct>(m, "IntVectorStruct")
        .def_readwrite("Data", &s::IntVectorStruct::Data);

    py::class_<s::InventoryItemStruct, s::Struct>(m, "InventoryItemStruct")
        .def_readwrite("Data", &s::InventoryItemStruct::Data);

    py::class_<s::LinearColorStruct, s::Struct>(m, "LinearColorStruct")
        .def_readwrite("Data", &s::LinearColorStruct::Data);

    py::class_<s::PropertyStruct, s::Struct>(m, "PropertyStruct")
        //.def_readwrite("Data", &s::PropertyStruct::Data) // TODO
        ;

    py::class_<s::QuatStruct, s::Struct>(m, "QuatStruct")
        .def_readwrite("Data", &s::QuatStruct::Data);

    py::class_<s::RailroadTrackPositionStruct, s::Struct>(m, "RailroadTrackPositionStruct")
        .def_readwrite("Data", &s::RailroadTrackPositionStruct::Data);

    py::class_<s::RotatorStruct, s::Struct>(m, "RotatorStruct")
        .def_readwrite("Data", &s::RotatorStruct::Data);

    py::class_<s::ScalarMaterialInputStruct, s::Struct>(m, "ScalarMaterialInputStruct")
        .def_readwrite("Data", &s::ScalarMaterialInputStruct::Data);

    py::class_<s::SoftClassPathStruct, s::Struct>(m, "SoftClassPathStruct")
        .def_readwrite("Data", &s::SoftClassPathStruct::Data);

    py::class_<s::Vector2DStruct, s::Struct>(m, "Vector2DStruct")
        .def_readwrite("Data", &s::Vector2DStruct::Data);

    py::class_<s::VectorMaterialInputStruct, s::Struct>(m, "VectorMaterialInputStruct")
        .def_readwrite("Data", &s::VectorMaterialInputStruct::Data);

    py::class_<s::VectorStruct, s::Struct>(m, "VectorStruct")
        .def_readwrite("Data", &s::VectorStruct::Data);

    py::class_<s::LBBalancerIndexingStruct, s::Struct>(m, "LBBalancerIndexingStruct")
        .def_readwrite("Data", &s::LBBalancerIndexingStruct::Data);
}
