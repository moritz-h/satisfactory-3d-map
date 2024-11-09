#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include "SatisfactorySave/GameTypes/UE/Engine/GameFramework/Actor.h"
#include "libsavepy_common.h"

namespace py = pybind11;
namespace s = SatisfactorySave;

void init_GameTypes_UE_GameFramework(py::module_& m) {
    py::class_<s::AActor, s::UObject, std::shared_ptr<s::AActor>>(m, "AActor")
        .def(py::init<>())
        .def_readwrite("Owner", &s::AActor::Owner)
        .def_readwrite("Components", &s::AActor::Components);
}
