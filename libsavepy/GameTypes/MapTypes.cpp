#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include "SatisfactorySave/GameTypes/MapTypes/Base/MapTypeList.h"
#include "SatisfactorySave/GameTypes/MapTypes/Base/MapTypeListAll.h"
#include "libsavepy_common.h"

namespace py = pybind11;
namespace s = SatisfactorySave;

void init_GameTypes_MapTypes(py::module_& m) {
    py::class_<s::MapTypeList>(m, "MapTypeList");

    py::class_<s::ByteMapTypeList, s::MapTypeList>(m, "ByteMapTypeList")
        .def(py::init<>())
        .def_readwrite("List", &s::ByteMapTypeList::List);

    py::class_<s::EnumMapTypeList, s::MapTypeList>(m, "EnumMapTypeList")
        .def(py::init<>())
        .def_readwrite("List", &s::EnumMapTypeList::List);

    py::class_<s::FloatMapTypeList, s::MapTypeList>(m, "FloatMapTypeList")
        .def(py::init<>())
        .def_readwrite("List", &s::FloatMapTypeList::List);

    py::class_<s::IntMapTypeList, s::MapTypeList>(m, "IntMapTypeList")
        .def(py::init<>())
        .def_readwrite("List", &s::IntMapTypeList::List);

    py::class_<s::NameMapTypeList, s::MapTypeList>(m, "NameMapTypeList")
        .def(py::init<>())
        .def_readwrite("List", &s::NameMapTypeList::List);

    py::class_<s::ObjectMapTypeList, s::MapTypeList>(m, "ObjectMapTypeList")
        .def(py::init<>())
        .def_readwrite("List", &s::ObjectMapTypeList::List);

    py::class_<s::StructMapTypeList, s::MapTypeList>(m, "StructMapTypeList")
        .def(py::init<s::FName>())
        .def_property("List",
            [](s::StructMapTypeList& m) -> const std::vector<std::unique_ptr<s::Struct>>& { return m.List; },
            [](s::StructMapTypeList& m, const std::vector<const s::Struct*>& v) {
                m.List.clear();
                m.List.reserve(v.size());
                for (const auto& i : v) {
                    m.List.push_back(std::move(i->clone()));
                }
            })
        .def_property_readonly("structName",
            [](s::StructMapTypeList& m) -> const s::FName& { return m.getStructName(); });
}
