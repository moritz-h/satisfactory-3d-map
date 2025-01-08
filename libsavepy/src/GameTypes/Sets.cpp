#include "SatisfactorySave/GameTypes/Sets/Base/Set.h"
#include "SatisfactorySave/GameTypes/Sets/Base/SetAll.h"
#include "libsavepy_common.h"

void init_GameTypes_Sets(py::module_& m) {
    py::class_<s::Set, std::shared_ptr<s::Set>>(m, "Set");

    py::class_<s::ObjectSet, s::Set, std::shared_ptr<s::ObjectSet>>(m, "ObjectSet")
        .def(py::init<>())
        .def_readwrite("Values", &s::ObjectSet::Values);

    py::class_<s::StructSet, s::Set, std::shared_ptr<s::StructSet>>(m, "StructSet")
        .def(py::init<s::FName>())
        .def_readwrite("Values", &s::StructSet::Values)
        .def_property_readonly("StructName",
            [](s::StructSet& s) -> const s::FName& { return s.getStructName(); });

    py::class_<s::UInt32Set, s::Set, std::shared_ptr<s::UInt32Set>>(m, "UInt32Set")
        .def(py::init<>())
        .def_readwrite("Values", &s::UInt32Set::Values);
}
