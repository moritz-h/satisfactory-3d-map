#include "SatisfactorySave/GameTypes/Arrays/Base/Array.h"
#include "SatisfactorySave/GameTypes/Arrays/Base/ArrayAll.h"
#include "libsavepy_common.h"

void init_GameTypes_Arrays(py::module_& m) {
    py::class_<s::Array, std::shared_ptr<s::Array>>(m, "Array");

    py::class_<s::BoolArray, s::Array, std::shared_ptr<s::BoolArray>>(m, "BoolArray")
        .def(py::init<>())
        .def_readwrite("Values", &s::BoolArray::Values);

    py::class_<s::ByteArray, s::Array, std::shared_ptr<s::ByteArray>>(m, "ByteArray")
        .def(py::init<>())
        .def_readwrite("Values", &s::ByteArray::Values);

    py::class_<s::EnumArray, s::Array, std::shared_ptr<s::EnumArray>>(m, "EnumArray")
        .def(py::init<>())
        .def_readwrite("Values", &s::EnumArray::Values);

    py::class_<s::FloatArray, s::Array, std::shared_ptr<s::FloatArray>>(m, "FloatArray")
        .def(py::init<>())
        .def_readwrite("Values", &s::FloatArray::Values);

    py::class_<s::Int64Array, s::Array, std::shared_ptr<s::Int64Array>>(m, "Int64Array")
        .def(py::init<>())
        .def_readwrite("Values", &s::Int64Array::Values);

    py::class_<s::Int8Array, s::Array, std::shared_ptr<s::Int8Array>>(m, "Int8Array")
        .def(py::init<>())
        .def_readwrite("Values", &s::Int8Array::Values);

    py::class_<s::IntArray, s::Array, std::shared_ptr<s::IntArray>>(m, "IntArray")
        .def(py::init<>())
        .def_readwrite("Values", &s::IntArray::Values);

    py::class_<s::InterfaceArray, s::Array, std::shared_ptr<s::InterfaceArray>>(m, "InterfaceArray")
        .def(py::init<>())
        .def_readwrite("Values", &s::InterfaceArray::Values);

    py::class_<s::NameArray, s::Array, std::shared_ptr<s::NameArray>>(m, "NameArray")
        .def(py::init<>())
        .def_readwrite("Values", &s::NameArray::Values);

    py::class_<s::ObjectArray, s::Array, std::shared_ptr<s::ObjectArray>>(m, "ObjectArray")
        .def(py::init<>())
        .def_readwrite("Values", &s::ObjectArray::Values);

    py::class_<s::SoftObjectArray, s::Array, std::shared_ptr<s::SoftObjectArray>>(m, "SoftObjectArray")
        .def(py::init<>())
        .def_readwrite("Values", &s::SoftObjectArray::Values);

    py::class_<s::StrArray, s::Array, std::shared_ptr<s::StrArray>>(m, "StrArray")
        .def(py::init<>())
        .def_readwrite("Values", &s::StrArray::Values);

    py::class_<s::StructArray, s::Array, std::shared_ptr<s::StructArray>>(m, "StructArray")
        .def(py::init<>())
        .def_readwrite("Values", &s::StructArray::Values)
        .def_property("Name",
            [](s::StructArray& a) -> s::FName& { return a.Name(); },
            [](s::StructArray& a, const s::FName& v) { a.Name() = v; })
        .def_property("StructName",
            [](s::StructArray& a) -> s::FName& { return a.StructName(); },
            [](s::StructArray& a, const s::FName& v) { a.StructName() = v; })
        .def_property("StructGuid",
            [](s::StructArray& a) -> s::FGuid& { return a.StructGuid(); },
            [](s::StructArray& a, const s::FGuid& v) { a.StructGuid() = v; });
}
