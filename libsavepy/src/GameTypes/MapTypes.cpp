#include "SatisfactorySave/GameTypes/MapTypes/Base/MapTypeList.h"
#include "SatisfactorySave/GameTypes/MapTypes/Base/MapTypeListAll.h"
#include "libsavepy_common.h"

void init_GameTypes_MapTypes(py::module_& m) {
    py::classh<s::MapTypeList>(m, "MapTypeList");

    py::classh<s::ByteMapTypeList, s::MapTypeList>(m, "ByteMapTypeList")
        .def(py::init<>())
        .def_readwrite("List", &s::ByteMapTypeList::List);

    py::classh<s::EnumMapTypeList, s::MapTypeList>(m, "EnumMapTypeList")
        .def(py::init<>())
        .def_readwrite("List", &s::EnumMapTypeList::List);

    py::classh<s::FloatMapTypeList, s::MapTypeList>(m, "FloatMapTypeList")
        .def(py::init<>())
        .def_readwrite("List", &s::FloatMapTypeList::List);

    py::classh<s::Int64MapTypeList, s::MapTypeList>(m, "Int64MapTypeList")
        .def(py::init<>())
        .def_readwrite("List", &s::Int64MapTypeList::List);

    py::classh<s::IntMapTypeList, s::MapTypeList>(m, "IntMapTypeList")
        .def(py::init<>())
        .def_readwrite("List", &s::IntMapTypeList::List);

    py::classh<s::NameMapTypeList, s::MapTypeList>(m, "NameMapTypeList")
        .def(py::init<>())
        .def_readwrite("List", &s::NameMapTypeList::List);

    py::classh<s::ObjectMapTypeList, s::MapTypeList>(m, "ObjectMapTypeList")
        .def(py::init<>())
        .def_readwrite("List", &s::ObjectMapTypeList::List);

    py::classh<s::SoftObjectMapTypeList, s::MapTypeList>(m, "SoftObjectMapTypeList")
        .def(py::init<>())
        .def_readwrite("List", &s::SoftObjectMapTypeList::List);

    py::classh<s::StructMapTypeList, s::MapTypeList>(m, "StructMapTypeList")
        .def(py::init<s::FName>())
        .def_readwrite("List", &s::StructMapTypeList::List)
        .def_property_readonly("StructName",
            [](s::StructMapTypeList& m) -> const s::FName& { return m.getStructName(); });
}
