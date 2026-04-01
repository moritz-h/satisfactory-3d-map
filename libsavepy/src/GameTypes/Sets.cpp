#include "SatisfactorySave/GameTypes/Sets/Base/Set.h"
#include "SatisfactorySave/GameTypes/Sets/Base/SetAll.h"
#include "libsavepy_common.h"

void init_GameTypes_Sets(py::module_& m) {
    py::classh<s::Set>(m, "Set");

    py::classh<s::ObjectSet, s::Set>(m, "ObjectSet")
        .def(py::init<>())
        .def_readwrite("Values", &s::ObjectSet::Values);

    py::classh<s::StructSet, s::Set>(m, "StructSet")
        .def(py::init<s::FName>())
        .def_readwrite("Values", &s::StructSet::Values)
        .def_property_readonly("StructName",
            [](s::StructSet& s) -> const s::FName& { return s.getStructName(); });

    py::classh<s::UInt32Set, s::Set>(m, "UInt32Set")
        .def(py::init<>())
        .def_readwrite("Values", &s::UInt32Set::Values);
}
