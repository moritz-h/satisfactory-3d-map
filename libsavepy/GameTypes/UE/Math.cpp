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
#include "libsavepy_common.h"

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
        .def(py::init([](uint8_t b, uint8_t g, uint8_t r, uint8_t a) {
            return s::FColor{b, g, r, a};
        }))
        .def(py::init([](const py::tuple& t) {
            return s::FColor{t[0].cast<uint8_t>(), t[1].cast<uint8_t>(), t[2].cast<uint8_t>(), t[3].cast<uint8_t>()};
        }))
        .def_readwrite("B", &s::FColor::B)
        .def_readwrite("G", &s::FColor::G)
        .def_readwrite("R", &s::FColor::R)
        .def_readwrite("A", &s::FColor::A);
    py::implicitly_convertible<py::tuple, s::FColor>();

    py::class_<s::FLinearColor>(m, "FLinearColor")
        .def(py::init<>())
        .def(py::init([](float r, float g, float b, float a) {
            return s::FLinearColor{r, g, b, a};
        }))
        .def(py::init([](const py::tuple& t) {
            return s::FLinearColor{t[0].cast<float>(), t[1].cast<float>(), t[2].cast<float>(), t[3].cast<float>()};
        }))
        .def_readwrite("R", &s::FLinearColor::R)
        .def_readwrite("G", &s::FLinearColor::G)
        .def_readwrite("B", &s::FLinearColor::B)
        .def_readwrite("A", &s::FLinearColor::A);
    py::implicitly_convertible<py::tuple, s::FLinearColor>();

    py::class_<s::FIntPoint>(m, "FIntPoint")
        .def(py::init<>())
        .def(py::init([](int32_t x, int32_t y) {
            return s::FIntPoint{x, y};
        }))
        .def(py::init([](const py::tuple& t) {
            return s::FIntPoint{t[0].cast<int32_t>(), t[1].cast<int32_t>()};
        }))
        .def_readwrite("X", &s::FIntPoint::X)
        .def_readwrite("Y", &s::FIntPoint::Y);
    py::implicitly_convertible<py::tuple, s::FIntPoint>();

    py::class_<s::FIntVector>(m, "FIntVector")
        .def(py::init<>())
        .def(py::init([](int32_t x, int32_t y, int32_t z) {
            return s::FIntVector{x, y, z};
        }))
        .def(py::init([](const py::tuple& t) {
            return s::FIntVector{t[0].cast<int32_t>(), t[1].cast<int32_t>(), t[2].cast<int32_t>()};
        }))
        .def_readwrite("X", &s::FIntVector::X)
        .def_readwrite("Y", &s::FIntVector::Y)
        .def_readwrite("Z", &s::FIntVector::Z);
    py::implicitly_convertible<py::tuple, s::FIntVector>();

    py::class_<s::FQuat>(m, "FQuat")
        .def(py::init<>())
        .def(py::init([](double x, double y, double z, double w) {
            return s::FQuat{x, y, z, w};
        }))
        .def(py::init([](const py::tuple& t) {
            return s::FQuat{t[0].cast<double>(), t[1].cast<double>(), t[2].cast<double>(), t[3].cast<double>()};
        }))
        .def_readwrite("X", &s::FQuat::X)
        .def_readwrite("Y", &s::FQuat::Y)
        .def_readwrite("Z", &s::FQuat::Z)
        .def_readwrite("W", &s::FQuat::W);
    py::implicitly_convertible<py::tuple, s::FQuat>();

    py::class_<s::FQuat4f>(m, "FQuat4f")
        .def(py::init<>())
        .def(py::init([](float x, float y, float z, float w) {
            return s::FQuat4f{x, y, z, w};
        }))
        .def(py::init([](const py::tuple& t) {
            return s::FQuat4f{t[0].cast<float>(), t[1].cast<float>(), t[2].cast<float>(), t[3].cast<float>()};
        }))
        .def_readwrite("X", &s::FQuat4f::X)
        .def_readwrite("Y", &s::FQuat4f::Y)
        .def_readwrite("Z", &s::FQuat4f::Z)
        .def_readwrite("W", &s::FQuat4f::W);
    py::implicitly_convertible<py::tuple, s::FQuat4f>();

    py::class_<s::FRotator>(m, "FRotator")
        .def(py::init<>())
        .def(py::init([](double pitch, double yaw, double roll) {
            return s::FRotator{pitch, yaw, roll};
        }))
        .def(py::init([](const py::tuple& t) {
            return s::FRotator{t[0].cast<double>(), t[1].cast<double>(), t[2].cast<double>()};
        }))
        .def_readwrite("Pitch", &s::FRotator::Pitch)
        .def_readwrite("Yaw", &s::FRotator::Yaw)
        .def_readwrite("Roll", &s::FRotator::Roll)
        .def("Quaternion", &s::FRotator::Quaternion);
    py::implicitly_convertible<py::tuple, s::FRotator>();

    py::class_<s::FTransform3f>(m, "FTransform3f")
        .def(py::init<>())
        .def_readwrite("Rotation", &s::FTransform3f::Rotation)
        .def_readwrite("Translation", &s::FTransform3f::Translation)
        .def_readwrite("Scale3D", &s::FTransform3f::Scale3D);

    py::class_<s::FVector>(m, "FVector")
        .def(py::init<>())
        .def(py::init([](double x, double y, double z) {
            return s::FVector{x, y, z};
        }))
        .def(py::init([](const py::tuple& t) {
            return s::FVector{t[0].cast<double>(), t[1].cast<double>(), t[2].cast<double>()};
        }))
        .def_readwrite("X", &s::FVector::X)
        .def_readwrite("Y", &s::FVector::Y)
        .def_readwrite("Z", &s::FVector::Z);
    py::implicitly_convertible<py::tuple, s::FVector>();

    py::class_<s::FVector3f>(m, "FVector3f")
        .def(py::init<>())
        .def(py::init([](float x, float y, float z) {
            return s::FVector3f{x, y, z};
        }))
        .def(py::init([](const py::tuple& t) {
            return s::FVector3f{t[0].cast<float>(), t[1].cast<float>(), t[2].cast<float>()};
        }))
        .def_readwrite("X", &s::FVector3f::X)
        .def_readwrite("Y", &s::FVector3f::Y)
        .def_readwrite("Z", &s::FVector3f::Z);
    py::implicitly_convertible<py::tuple, s::FVector3f>();

    py::class_<s::FVector2D>(m, "FVector2D")
        .def(py::init<>())
        .def(py::init([](double x, double y) {
            return s::FVector2D{x, y};
        }))
        .def(py::init([](const py::tuple& t) {
            return s::FVector2D{t[0].cast<double>(), t[1].cast<double>()};
        }))
        .def_readwrite("X", &s::FVector2D::X)
        .def_readwrite("Y", &s::FVector2D::Y);
    py::implicitly_convertible<py::tuple, s::FVector2D>();
}
