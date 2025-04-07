#include "SatisfactorySave/GameTypes/FactoryGamePlugins/OnlineIntegration/LocalUserInfo.h"
#include "libsavepy_common.h"

void init_GameTypes_FactoryGamePlugins(py::module_& m) {
    py::class_<s::FLocalUserNetIdBundle>(m, "FLocalUserNetIdBundle")
        .def(py::init<>())
        .def_readwrite("AssociatedAccountIdString", &s::FLocalUserNetIdBundle::AssociatedAccountIdString)
        .def_readwrite("DisplayName", &s::FLocalUserNetIdBundle::DisplayName)
        .def_readwrite("BackendName", &s::FLocalUserNetIdBundle::BackendName);
}
