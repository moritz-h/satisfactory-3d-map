#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include "SatisfactorySave/GameTypes/UE/Math/Box.h"
#include "SatisfactorySave/GameTypes/UE/Math/Color.h"
#include "SatisfactorySave/GameTypes/UE/Math/IntPoint.h"
#include "SatisfactorySave/GameTypes/UE/Math/IntVector.h"
#include "SatisfactorySave/GameTypes/UE/Math/Quat.h"
#include "SatisfactorySave/GameTypes/UE/Math/Rotator.h"
#include "SatisfactorySave/GameTypes/UE/Math/Transform.h"
#include "SatisfactorySave/GameTypes/UE/Math/Vector.h"
#include "SatisfactorySave/GameTypes/UE/Math/Vector2D.h"

namespace py = pybind11;
namespace s = SatisfactorySave;

void init_GameTypes_UE_Math(py::module_& m) {
    py::class_<s::FBox>(m, "FBox")
        .def(py::init<>())
        .def_readwrite("Min", &s::FBox::Min)
        .def_readwrite("Max", &s::FBox::Max)
        .def_readwrite("IsValid", &s::FBox::IsValid);

    py::class_<s::FColor>(m, "FColor")
        .def(py::init<>())
        .def_readwrite("B", &s::FColor::B)
        .def_readwrite("G", &s::FColor::G)
        .def_readwrite("R", &s::FColor::R)
        .def_readwrite("A", &s::FColor::A);

    py::class_<s::FLinearColor>(m, "FLinearColor")
        .def(py::init<>())
        .def_readwrite("R", &s::FLinearColor::R)
        .def_readwrite("G", &s::FLinearColor::G)
        .def_readwrite("B", &s::FLinearColor::B)
        .def_readwrite("A", &s::FLinearColor::A);

    py::class_<s::FIntPoint>(m, "FIntPoint")
        .def(py::init<>())
        .def_readwrite("X", &s::FIntPoint::X)
        .def_readwrite("Y", &s::FIntPoint::Y);

    py::class_<s::FIntVector>(m, "FIntVector")
        .def(py::init<>())
        .def_readwrite("X", &s::FIntVector::X)
        .def_readwrite("Y", &s::FIntVector::Y)
        .def_readwrite("Z", &s::FIntVector::Z);

    py::class_<s::FQuat>(m, "FQuat")
        .def(py::init<>())
        .def_readwrite("X", &s::FQuat::X)
        .def_readwrite("Y", &s::FQuat::Y)
        .def_readwrite("Z", &s::FQuat::Z)
        .def_readwrite("W", &s::FQuat::W);

    py::class_<s::FRotator>(m, "FRotator")
        .def(py::init<>())
        .def_readwrite("Pitch", &s::FRotator::Pitch)
        .def_readwrite("Yaw", &s::FRotator::Yaw)
        .def_readwrite("Roll", &s::FRotator::Roll)
        .def("Quaternion", &s::FRotator::Quaternion);

    py::class_<s::FTransform3f>(m, "FTransform3f")
        .def(py::init<>())
        .def_readwrite("Rotation", &s::FTransform3f::Rotation)
        .def_readwrite("Translation", &s::FTransform3f::Translation)
        .def_readwrite("Scale3D", &s::FTransform3f::Scale3D);

    py::class_<s::FVector>(m, "FVector")
        .def(py::init<>())
        .def_readwrite("X", &s::FVector::X)
        .def_readwrite("Y", &s::FVector::Y)
        .def_readwrite("Z", &s::FVector::Z);

    py::class_<s::FVector3f>(m, "FVector3f")
        .def(py::init<>())
        .def_readwrite("X", &s::FVector3f::X)
        .def_readwrite("Y", &s::FVector3f::Y)
        .def_readwrite("Z", &s::FVector3f::Z);

    py::class_<s::FVector2D>(m, "FVector2D")
        .def(py::init<>())
        .def_readwrite("X", &s::FVector2D::X)
        .def_readwrite("Y", &s::FVector2D::Y);
}
