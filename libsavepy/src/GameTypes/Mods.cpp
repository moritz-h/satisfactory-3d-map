#include "SatisfactorySave/GameTypes/Mods/LBBalancerIndexing.h"
#include "libsavepy_common.h"

void init_GameTypes_Mods(py::module_& m) {
    py::class_<s::FLBBalancerIndexing>(m, "FLBBalancerIndexing")
        .def(py::init<>())
        .def_readwrite("mNormalIndex", &s::FLBBalancerIndexing::mNormalIndex)
        .def_readwrite("mOverflowIndex", &s::FLBBalancerIndexing::mOverflowIndex)
        .def_readwrite("mFilterIndex", &s::FLBBalancerIndexing::mFilterIndex);
}
