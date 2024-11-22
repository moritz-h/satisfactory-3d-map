#include "SatisfactorySave/GameTypes/MapTypes/Base/MapTypeList.h"
#include "SatisfactorySave/GameTypes/MapTypes/Base/MapTypeListAll.h"
#include "libsavepy_common.h"

void init_GameTypes_MapTypes(py::module_& m) {
    py::class_<s::MapTypeList, std::shared_ptr<s::MapTypeList>>(m, "MapTypeList");

    py::class_<s::ByteMapTypeList, s::MapTypeList, std::shared_ptr<s::ByteMapTypeList>>(m, "ByteMapTypeList")
        .def(py::init<>())
        .def_readwrite("List", &s::ByteMapTypeList::List);

    py::class_<s::EnumMapTypeList, s::MapTypeList, std::shared_ptr<s::EnumMapTypeList>>(m, "EnumMapTypeList")
        .def(py::init<>())
        .def_readwrite("List", &s::EnumMapTypeList::List);

    py::class_<s::FloatMapTypeList, s::MapTypeList, std::shared_ptr<s::FloatMapTypeList>>(m, "FloatMapTypeList")
        .def(py::init<>())
        .def_readwrite("List", &s::FloatMapTypeList::List);

    py::class_<s::Int64MapTypeList, s::MapTypeList, std::shared_ptr<s::Int64MapTypeList>>(m, "Int64MapTypeList")
        .def(py::init<>())
        .def_readwrite("List", &s::Int64MapTypeList::List);

    py::class_<s::IntMapTypeList, s::MapTypeList, std::shared_ptr<s::IntMapTypeList>>(m, "IntMapTypeList")
        .def(py::init<>())
        .def_readwrite("List", &s::IntMapTypeList::List);

    py::class_<s::NameMapTypeList, s::MapTypeList, std::shared_ptr<s::NameMapTypeList>>(m, "NameMapTypeList")
        .def(py::init<>())
        .def_readwrite("List", &s::NameMapTypeList::List);

    py::class_<s::ObjectMapTypeList, s::MapTypeList, std::shared_ptr<s::ObjectMapTypeList>>(m, "ObjectMapTypeList")
        .def(py::init<>())
        .def_readwrite("List", &s::ObjectMapTypeList::List);

    py::class_<s::StructMapTypeList, s::MapTypeList, std::shared_ptr<s::StructMapTypeList>>(m, "StructMapTypeList")
        .def(py::init<s::FName>())
        .def_readwrite("List", &s::StructMapTypeList::List)
        .def_property_readonly("StructName",
            [](s::StructMapTypeList& m) -> const s::FName& { return m.getStructName(); });
}
