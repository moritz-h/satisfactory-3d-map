#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include "SatisfactorySave/GameTypes/UE/Engine/Components/ActorComponent.h"
#include "SatisfactorySave/GameTypes/UE/Engine/Components/SplineComponent.h"
#include "SatisfactorySave/GameTypes/UE/Engine/Engine/ReplicatedState.h"
#include "libsavepy_common.h"

namespace py = pybind11;
namespace s = SatisfactorySave;

void init_GameTypes_UE_Engine(py::module_& m) {
    py::class_<s::UActorComponent, s::UObject, std::shared_ptr<s::UActorComponent>>(m, "UActorComponent")
        .def(py::init<>());

    py::class_<s::FRigidBodyState>(m, "FRigidBodyState")
        .def(py::init<>())
        .def_readwrite("Position", &s::FRigidBodyState::Position)
        .def_readwrite("Quaternion", &s::FRigidBodyState::Quaternion)
        .def_readwrite("LinVel", &s::FRigidBodyState::LinVel)
        .def_readwrite("AngVel", &s::FRigidBodyState::AngVel)
        .def_readwrite("Flags", &s::FRigidBodyState::Flags);

    py::class_<s::FSplinePointData>(m, "FSplinePointData")
        .def(py::init<>())
        .def_readwrite("Location", &s::FSplinePointData::Location)
        .def_readwrite("ArriveTangent", &s::FSplinePointData::ArriveTangent)
        .def_readwrite("LeaveTangent", &s::FSplinePointData::LeaveTangent);
}
