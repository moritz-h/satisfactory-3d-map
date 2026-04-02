#include "SatisfactorySave/GameTypes/Structs/Base/Struct.h"
#include "SatisfactorySave/GameTypes/Structs/Base/StructAll.h"
#include "libsavepy_common.h"

void init_GameTypes_Structs(py::module_& m) {
    py::classh<s::Struct>(m, "Struct")
        .def_property_readonly("StructName",
            [](s::Struct& s) -> const s::FName& { return s.getStructName(); });

    py::classh<s::Box3fStruct, s::Struct>(m, "Box3fStruct")
        .def(py::init<>())
        .def_readwrite("Data", &s::Box3fStruct::Data);

    py::classh<s::BoxStruct, s::Struct>(m, "BoxStruct")
        .def(py::init<>())
        .def_readwrite("Data", &s::BoxStruct::Data);

    py::classh<s::ClientIdentityInfoStruct, s::Struct>(m, "ClientIdentityInfoStruct")
        .def(py::init<>())
        .def_readwrite("Data", &s::ClientIdentityInfoStruct::Data);

    py::classh<s::ColorStruct, s::Struct>(m, "ColorStruct")
        .def(py::init<>())
        .def_readwrite("Data", &s::ColorStruct::Data);

    py::classh<s::DateTimeStruct, s::Struct>(m, "DateTimeStruct")
        .def(py::init<>())
        .def_readwrite("Data", &s::DateTimeStruct::Data);

    py::classh<s::FluidBoxStruct, s::Struct>(m, "FluidBoxStruct")
        .def(py::init<>())
        .def_readwrite("Data", &s::FluidBoxStruct::Data);

    py::classh<s::GuidStruct, s::Struct>(m, "GuidStruct")
        .def(py::init<>())
        .def_readwrite("Data", &s::GuidStruct::Data);

    py::classh<s::IntPointStruct, s::Struct>(m, "IntPointStruct")
        .def(py::init<>())
        .def_readwrite("Data", &s::IntPointStruct::Data);

    py::classh<s::IntVectorStruct, s::Struct>(m, "IntVectorStruct")
        .def(py::init<>())
        .def_readwrite("Data", &s::IntVectorStruct::Data);

    py::classh<s::InventoryItemStruct, s::Struct>(m, "InventoryItemStruct")
        .def(py::init<>())
        .def_readwrite("Data", &s::InventoryItemStruct::Data);

    py::classh<s::LinearColorStruct, s::Struct>(m, "LinearColorStruct")
        .def(py::init<>())
        .def_readwrite("Data", &s::LinearColorStruct::Data);

    py::classh<s::PlayerInfoHandleStruct, s::Struct>(m, "PlayerInfoHandleStruct")
        .def(py::init<>())
        .def_readwrite("Data", &s::PlayerInfoHandleStruct::Data);

    py::classh<s::PropertyStruct, s::Struct>(m, "PropertyStruct")
        .def(py::init<s::FName>())
        .def_readwrite("Data", &s::PropertyStruct::Data);

    py::classh<s::QuatStruct, s::Struct>(m, "QuatStruct")
        .def(py::init<>())
        .def_readwrite("Data", &s::QuatStruct::Data);

    py::classh<s::RailroadTrackPositionStruct, s::Struct>(m, "RailroadTrackPositionStruct")
        .def(py::init<>())
        .def_readwrite("Data", &s::RailroadTrackPositionStruct::Data);

    py::classh<s::RichCurveKeyStruct, s::Struct>(m, "RichCurveKeyStruct")
        .def(py::init<>())
        .def_readwrite("Data", &s::RichCurveKeyStruct::Data);

    py::classh<s::RotatorStruct, s::Struct>(m, "RotatorStruct")
        .def(py::init<>())
        .def_readwrite("Data", &s::RotatorStruct::Data);

    py::classh<s::ScalarMaterialInputStruct, s::Struct>(m, "ScalarMaterialInputStruct")
        .def(py::init<>())
        .def_readwrite("Data", &s::ScalarMaterialInputStruct::Data);

    py::classh<s::SoftClassPathStruct, s::Struct>(m, "SoftClassPathStruct")
        .def(py::init<>())
        .def_readwrite("Data", &s::SoftClassPathStruct::Data);

    py::classh<s::UniqueNetIdReplStruct, s::Struct>(m, "UniqueNetIdReplStruct")
        .def(py::init<>())
        .def_readwrite("Data", &s::UniqueNetIdReplStruct::Data);

    py::classh<s::Vector2DStruct, s::Struct>(m, "Vector2DStruct")
        .def(py::init<>())
        .def_readwrite("Data", &s::Vector2DStruct::Data);

    py::classh<s::Vector4Struct, s::Struct>(m, "Vector4Struct")
        .def(py::init<>())
        .def_readwrite("Data", &s::Vector4Struct::Data);

    py::classh<s::VectorMaterialInputStruct, s::Struct>(m, "VectorMaterialInputStruct")
        .def(py::init<>())
        .def_readwrite("Data", &s::VectorMaterialInputStruct::Data);

    py::classh<s::VectorStruct, s::Struct>(m, "VectorStruct")
        .def(py::init<>())
        .def_readwrite("Data", &s::VectorStruct::Data);

    py::classh<s::LBBalancerIndexingStruct, s::Struct>(m, "LBBalancerIndexingStruct")
        .def(py::init<>())
        .def_readwrite("Data", &s::LBBalancerIndexingStruct::Data);
}
