#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include "SatisfactorySave/GameTypes/UE/Internationalization/Text.h"

namespace py = pybind11;
namespace s = SatisfactorySave;

void init_GameTypes_UE_Internationalization(py::module_& m) {
    py::class_<s::FText>(m, "FText")
        .def("string", &s::FText::string);
}
