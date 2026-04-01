#include "SatisfactorySave/GameTypes/Arrays/Base/Array.h"
#include "SatisfactorySave/GameTypes/Arrays/Base/ArrayAll.h"
#include "libsavepy_common.h"

void init_GameTypes_Arrays(py::module_& m) {
    py::classh<s::Array>(m, "Array");

    py::classh<s::BoolArray, s::Array>(m, "BoolArray")
        .def(py::init<>())
        .def_readwrite("Values", &s::BoolArray::Values);

    py::classh<s::ByteArray, s::Array>(m, "ByteArray")
        .def(py::init<>())
        .def_readwrite("Values", &s::ByteArray::Values);

    py::classh<s::DoubleArray, s::Array>(m, "DoubleArray")
        .def(py::init<>())
        .def_readwrite("Values", &s::DoubleArray::Values);

    py::classh<s::EnumArray, s::Array>(m, "EnumArray")
        .def(py::init<>())
        .def_readwrite("Values", &s::EnumArray::Values);

    py::classh<s::FloatArray, s::Array>(m, "FloatArray")
        .def(py::init<>())
        .def_readwrite("Values", &s::FloatArray::Values);

    py::classh<s::Int64Array, s::Array>(m, "Int64Array")
        .def(py::init<>())
        .def_readwrite("Values", &s::Int64Array::Values);

    py::classh<s::Int8Array, s::Array>(m, "Int8Array")
        .def(py::init<>())
        .def_readwrite("Values", &s::Int8Array::Values);

    py::classh<s::IntArray, s::Array>(m, "IntArray")
        .def(py::init<>())
        .def_readwrite("Values", &s::IntArray::Values);

    py::classh<s::InterfaceArray, s::Array>(m, "InterfaceArray")
        .def(py::init<>())
        .def_readwrite("Values", &s::InterfaceArray::Values);

    py::classh<s::NameArray, s::Array>(m, "NameArray")
        .def(py::init<>())
        .def_readwrite("Values", &s::NameArray::Values);

    py::classh<s::ObjectArray, s::Array>(m, "ObjectArray")
        .def(py::init<>())
        .def_readwrite("Values", &s::ObjectArray::Values);

    py::classh<s::SoftObjectArray, s::Array>(m, "SoftObjectArray")
        .def(py::init<>())
        .def_readwrite("Values", &s::SoftObjectArray::Values);

    py::classh<s::StrArray, s::Array>(m, "StrArray")
        .def(py::init<>())
        .def_readwrite("Values", &s::StrArray::Values);

    py::classh<s::StructArray, s::Array>(m, "StructArray")
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
