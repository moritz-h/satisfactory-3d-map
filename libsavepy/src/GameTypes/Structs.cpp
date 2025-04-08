#include "SatisfactorySave/GameTypes/Structs/Base/Struct.h"
#include "SatisfactorySave/GameTypes/Structs/Base/StructAll.h"
#include "libsavepy_common.h"

void init_GameTypes_Structs(py::module_& m) {
    py::class_<s::Struct, std::shared_ptr<s::Struct>>(m, "Struct")
        .def_property_readonly("StructName",
            [](s::Struct& s) -> const s::FName& { return s.getStructName(); });

    py::class_<s::BoxStruct, s::Struct, std::shared_ptr<s::BoxStruct>>(m, "BoxStruct")
        .def(py::init<>())
        .def_readwrite("Data", &s::BoxStruct::Data);

    py::class_<s::ClientIdentityInfoStruct, s::Struct, std::shared_ptr<s::ClientIdentityInfoStruct>>(m, "ClientIdentityInfoStruct")
        .def(py::init<>())
        .def_readwrite("Data", &s::ClientIdentityInfoStruct::Data);

    py::class_<s::ColorStruct, s::Struct, std::shared_ptr<s::ColorStruct>>(m, "ColorStruct")
        .def(py::init<>())
        .def_readwrite("Data", &s::ColorStruct::Data);

    py::class_<s::DateTimeStruct, s::Struct, std::shared_ptr<s::DateTimeStruct>>(m, "DateTimeStruct")
        .def(py::init<>())
        .def_readwrite("Data", &s::DateTimeStruct::Data);

    py::class_<s::FluidBoxStruct, s::Struct, std::shared_ptr<s::FluidBoxStruct>>(m, "FluidBoxStruct")
        .def(py::init<>())
        .def_readwrite("Data", &s::FluidBoxStruct::Data);

    py::class_<s::GuidStruct, s::Struct, std::shared_ptr<s::GuidStruct>>(m, "GuidStruct")
        .def(py::init<>())
        .def_readwrite("Data", &s::GuidStruct::Data);

    py::class_<s::IntPointStruct, s::Struct, std::shared_ptr<s::IntPointStruct>>(m, "IntPointStruct")
        .def(py::init<>())
        .def_readwrite("Data", &s::IntPointStruct::Data);

    py::class_<s::IntVectorStruct, s::Struct, std::shared_ptr<s::IntVectorStruct>>(m, "IntVectorStruct")
        .def(py::init<>())
        .def_readwrite("Data", &s::IntVectorStruct::Data);

    py::class_<s::InventoryItemStruct, s::Struct, std::shared_ptr<s::InventoryItemStruct>>(m, "InventoryItemStruct")
        .def(py::init<>())
        .def_readwrite("Data", &s::InventoryItemStruct::Data);

    py::class_<s::LinearColorStruct, s::Struct, std::shared_ptr<s::LinearColorStruct>>(m, "LinearColorStruct")
        .def(py::init<>())
        .def_readwrite("Data", &s::LinearColorStruct::Data);

    py::class_<s::PropertyStruct, s::Struct, std::shared_ptr<s::PropertyStruct>>(m, "PropertyStruct")
        .def(py::init<s::FName>())
        .def_readwrite("Data", &s::PropertyStruct::Data);

    py::class_<s::QuatStruct, s::Struct, std::shared_ptr<s::QuatStruct>>(m, "QuatStruct")
        .def(py::init<>())
        .def_readwrite("Data", &s::QuatStruct::Data);

    py::class_<s::RailroadTrackPositionStruct, s::Struct, std::shared_ptr<s::RailroadTrackPositionStruct>>(m, "RailroadTrackPositionStruct")
        .def(py::init<>())
        .def_readwrite("Data", &s::RailroadTrackPositionStruct::Data);

    py::class_<s::RotatorStruct, s::Struct, std::shared_ptr<s::RotatorStruct>>(m, "RotatorStruct")
        .def(py::init<>())
        .def_readwrite("Data", &s::RotatorStruct::Data);

    py::class_<s::ScalarMaterialInputStruct, s::Struct, std::shared_ptr<s::ScalarMaterialInputStruct>>(m, "ScalarMaterialInputStruct")
        .def(py::init<>())
        .def_readwrite("Data", &s::ScalarMaterialInputStruct::Data);

    py::class_<s::SoftClassPathStruct, s::Struct, std::shared_ptr<s::SoftClassPathStruct>>(m, "SoftClassPathStruct")
        .def(py::init<>())
        .def_readwrite("Data", &s::SoftClassPathStruct::Data);

    py::class_<s::Vector2DStruct, s::Struct, std::shared_ptr<s::Vector2DStruct>>(m, "Vector2DStruct")
        .def(py::init<>())
        .def_readwrite("Data", &s::Vector2DStruct::Data);

    py::class_<s::Vector4Struct, s::Struct, std::shared_ptr<s::Vector4Struct>>(m, "Vector4Struct")
        .def(py::init<>())
        .def_readwrite("Data", &s::Vector4Struct::Data);

    py::class_<s::VectorMaterialInputStruct, s::Struct, std::shared_ptr<s::VectorMaterialInputStruct>>(m, "VectorMaterialInputStruct")
        .def(py::init<>())
        .def_readwrite("Data", &s::VectorMaterialInputStruct::Data);

    py::class_<s::VectorStruct, s::Struct, std::shared_ptr<s::VectorStruct>>(m, "VectorStruct")
        .def(py::init<>())
        .def_readwrite("Data", &s::VectorStruct::Data);

    py::class_<s::LBBalancerIndexingStruct, s::Struct, std::shared_ptr<s::LBBalancerIndexingStruct>>(m, "LBBalancerIndexingStruct")
        .def(py::init<>())
        .def_readwrite("Data", &s::LBBalancerIndexingStruct::Data);
}
