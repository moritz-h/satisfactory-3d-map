#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include "SatisfactorySave/GameTypes/UE/UObject/Name.h"
#include "SatisfactorySave/GameTypes/UE/UObject/SoftObjectPath.h"
#include "libsavepy_common.h"

namespace py = pybind11;
namespace s = SatisfactorySave;

void init_GameTypes_UE_UObject(py::module_& m) {
    py::class_<s::FName>(m, "FName")
        .def(py::init<>())
        .def_readwrite("Name", &s::FName::Name)
        .def_readwrite("Number", &s::FName::Number)
        .def("toString", &s::FName::toString);

    py::class_<s::FSoftObjectPath>(m, "FSoftObjectPath")
        .def(py::init<>())
        .def("AssetPathName", &s::FSoftObjectPath::AssetPathName)
        .def("SubPathString", &s::FSoftObjectPath::SubPathString);
}
