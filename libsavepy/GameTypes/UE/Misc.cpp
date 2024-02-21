#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include "SatisfactorySave/GameTypes/UE/Misc/Guid.h"
#include "SatisfactorySave/GameTypes/UE/Misc/Hash.h"
#include "libsavepy_common.h"

namespace py = pybind11;
namespace s = SatisfactorySave;

void init_GameTypes_UE_Misc(py::module_& m) {
    py::class_<s::FGuid>(m, "FGuid")
        .def(py::init<>())
        .def("isZero", &s::FGuid::isZero)
        .def("toString", &s::FGuid::toString);

    py::class_<s::FMD5Hash>(m, "FMD5Hash")
        .def(py::init<>())
        .def("toString", &s::FMD5Hash::toString);

    py::class_<s::FSHAHash>(m, "FSHAHash")
        .def(py::init<>());
}
