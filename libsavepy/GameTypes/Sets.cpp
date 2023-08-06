#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include "SatisfactorySave/GameTypes/Sets/Base/Set.h"
#include "SatisfactorySave/GameTypes/Sets/Base/SetAll.h"

namespace py = pybind11;
namespace s = SatisfactorySave;

void init_GameTypes_Sets(py::module_& m) {
    py::class_<s::Set>(m, "Set");

    py::class_<s::StructSet, s::Set>(m, "StructSet")
        //.def_readwrite("Set", &s::StructSet::Set) // TODO
        .def("structName", &s::StructSet::structName);

    py::class_<s::UInt32Set, s::Set>(m, "UInt32Set")
        .def_readwrite("Set", &s::UInt32Set::Set);
}
